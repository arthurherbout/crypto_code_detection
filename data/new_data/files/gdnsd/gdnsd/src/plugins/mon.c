/* Copyright © 2012 Brandon L Black <blblack@gmail.com>
 *
 * This file is part of gdnsd.
 *
 * gdnsd is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * gdnsd is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with gdnsd.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <config.h>
#include "mon.h"

#include <gdnsd/alloc.h>
#include <gdnsd/log.h>
#include <gdnsd/paths.h>
#include "plugapi.h"
#include <gdnsd/vscf.h>
#include <gdnsd/misc.h>

#include <string.h>
#include <unistd.h>
#include <fnmatch.h>

#include <ev.h>
#include <urcu-qsbr.h>

typedef struct {
    const char* name;
    const plugin_t* plugin;
    unsigned up_thresh;
    unsigned ok_thresh;
    unsigned down_thresh;
    unsigned interval;
    unsigned timeout;
} service_type_t;

// if type is NULL, there is no real monitoring,
//   it's only possible to administratively change
//   the state.  This means a virtual resource
//   via mon_add_admin(), and cname/addr/dname
//   are invalid.  Otherwise is_cname flags which
//   member of the union is valid.
typedef struct {
    const char* desc;
    service_type_t* type;
    char* cname; // normalized text form of addr or dname below
    union {
        gdnsd_anysin_t addr;
        const uint8_t* dname; // dname-form of a CNAME
    };
    unsigned n_failure;
    unsigned n_success;
    bool is_cname;
    gdnsd_sttl_t real_sttl;
} smgr_t;

static unsigned num_svc_types = 0;
static service_type_t* service_types = NULL;

static unsigned num_smgrs = 0;
static smgr_t* smgrs = NULL;

// There are two copies of the sttl table.
// The "consumer" copy is always ready for consumption
//   (via rcu deref) by other threads, and does not
//   get mutated directly.  The updates flow into
//   the non-consumer table and the tables are later
//   rcu swapped (with the old copy updated to new values)
// There are only ever the two chunks of memory that are
//   first allocated for these, they just get swapped around
//   and copied over each other.
// (see sttl_table_update() below)
static gdnsd_sttl_t* smgr_sttl = NULL;
gdnsd_sttl_t* smgr_sttl_consumer_ = NULL;

static size_t max_states_len = 0;

static bool initial_round = false;
static bool testsuite_nodelay = false;

static struct ev_loop* mon_loop = NULL;
static ev_timer sttl_update_timer;

#define DEF_UP_THRESH 20
#define DEF_OK_THRESH 10
#define DEF_DOWN_THRESH 10
#define DEF_INTERVAL 10

F_NONNULL
static void sttl_table_update(struct ev_loop* loop V_UNUSED, ev_timer* w V_UNUSED, int revents V_UNUSED)
{
    gdnsd_assert(w == &sttl_update_timer);
    gdnsd_assert(revents == EV_TIMER);

    // rcu-swap of the two tables
    gdnsd_sttl_t* saved_old_consumer = smgr_sttl_consumer_;
    rcu_assign_pointer(smgr_sttl_consumer_, smgr_sttl);
    synchronize_rcu();
    smgr_sttl = saved_old_consumer;

    // now copy the (new) consumer table back over the old one
    //   that we're using for future offline updates until the next swap
    memcpy(smgr_sttl, smgr_sttl_consumer_, sizeof(*smgr_sttl) * num_smgrs);
}

// anything that ends up changing a value in smgr_sttl[] calls
//   this to push the updates towards visibility to consumers.
// the timer coalesces rapid-fire updates into at most one table-swap
//   per second, at the cost of a second of latency on updates.
static void kick_sttl_update_timer(void)
{
    ev_timer* sut = &sttl_update_timer;
    if (testsuite_nodelay) {
        sttl_table_update(mon_loop, sut, EV_TIMER);
    } else if (!ev_is_active(sut) && !ev_is_pending(sut)) {
        ev_timer_set(sut, 1.0, 0.0);
        ev_timer_start(mon_loop, sut);
    }
}

const char* gdnsd_logf_sttl(const gdnsd_sttl_t s)
{
    // the maximal length here is "DOWN/268435455"
    // the minimal is "UP/1"
    // 4-14 bytes not counting NUL
    char tmpbuf[15];
    const unsigned ttl = s & GDNSD_STTL_TTL_MASK;
    const char* state = (s & GDNSD_STTL_DOWN) ? "DOWN" : "UP";
    int snp_rv;
    if (!ttl || ttl == GDNSD_STTL_TTL_MAX)
        snp_rv = snprintf(tmpbuf, 15, "%s/%s", state, ttl ? "MAX" : "MIN");
    else
        snp_rv = snprintf(tmpbuf, 15, "%s/%u", state, ttl);

    gdnsd_assert(snp_rv >= 4 && snp_rv <= 14);
    const unsigned snp_len = (unsigned)snp_rv;
    char* out = gdnsd_fmtbuf_alloc(snp_len + 1U);
    memcpy(out, tmpbuf, snp_len + 1U);
    return out;
}

//--------------------------------------------------
// admin state-force stuff
//--------------------------------------------------

static ev_stat admin_file_watcher;
static ev_timer admin_quiesce_timer;

// shared with plugin_extfile!
bool gdnsd_mon_parse_sttl(const char* sttl_str, gdnsd_sttl_t* sttl_out, unsigned def_ttl)
{
    bool failed = true;
    gdnsd_sttl_t out = def_ttl;
    assert_valid_sttl(out);

    const char* ttl_suffix = NULL;
    if (!strncasecmp(sttl_str, "UP", 2)) {
        ttl_suffix = sttl_str + 2;
    } else if (!strncasecmp(sttl_str, "DOWN", 4)) {
        out |= GDNSD_STTL_DOWN;
        ttl_suffix = sttl_str + 4;
    }

    if (ttl_suffix) {
        char slash = *ttl_suffix++;
        if (!slash) {
            failed = false; // no TTL suffix
            *sttl_out = out;
        } else if (slash == '/' && *ttl_suffix) {
            char* endptr = NULL;
            unsigned long ttl_tmp = strtoul(ttl_suffix, &endptr, 10);
            // strtoul finished the string successfully and value is in range
            if (endptr && !*endptr && ttl_tmp <= GDNSD_STTL_TTL_MAX) {
                out = (out & ~GDNSD_STTL_TTL_MASK) | ttl_tmp;
                assert_valid_sttl(out);
                *sttl_out = out;
                failed = false;
            }
        }
    }

    return failed;
}

F_NONNULL
static bool admin_process_entry(const char* matchme, gdnsd_sttl_t* updates, gdnsd_sttl_t update_val)
{
    assert_valid_sttl(update_val);
    gdnsd_assert(update_val & GDNSD_STTL_FORCED);

    bool success = true;
    bool matched = false;

    for (unsigned i = 0; i < num_smgrs; i++) {
        smgr_t* smgr = &smgrs[i];
        int err = fnmatch(matchme, smgr->desc, 0);
        if (err && err != FNM_NOMATCH) {
            log_err("admin_state: fnmatch() failed with error code %i: probably glob-parsing error on '%s'", err, matchme);
            success = false;
            break;
        }
        if (!err) { // matched!
            matched = true;
            updates[i] = update_val;
        }
    }

    if (success && !matched)
        log_warn("admin_state: glob '%s' did not match anything!", matchme);

    return success;
}

F_NONNULL
static bool admin_process_hash(vscf_data_t* raw, const bool check_only)
{
    gdnsd_assert(vscf_is_hash(raw));

    if (!num_smgrs)
        return true;

    gdnsd_sttl_t* updates = xcalloc_n(num_smgrs, sizeof(*updates));

    const unsigned num_raw = vscf_hash_get_len(raw);
    for (unsigned i = 0; i < num_raw; i++) {
        const char* matchme = vscf_hash_get_key_byindex(raw, i, NULL);
        vscf_data_t* val = vscf_hash_get_data_byindex(raw, i);
        if (!vscf_is_simple(val)) {
            log_err("admin_state: value for '%s' must be a simple string!", matchme);
            free(updates);
            return false;
        } else {
            gdnsd_sttl_t update_val;
            if (gdnsd_mon_parse_sttl(vscf_simple_get_data(val), &update_val, GDNSD_STTL_TTL_MAX)) {
                log_err("admin_state: value for '%s' must be of the form STATE[/TTL] (where STATE is 'UP' or 'DOWN', and the optional TTL is an unsigned integer in the range 0 - %u)", matchme, GDNSD_STTL_TTL_MAX);
                free(updates);
                return false;
            } else {
                update_val |= GDNSD_STTL_FORCED; // all admin-states are forced
                if (!admin_process_entry(matchme, updates, update_val)) {
                    free(updates);
                    return false;
                }
            }
        }
    }

    if (!check_only) {
        bool affected = false;

        for (unsigned i = 0; i < num_smgrs; i++) {
            if (updates[i]) { // some entry wants to affect this slot
                if (smgr_sttl[i] != updates[i]) { // new state change
                    if (smgr_sttl[i] != smgrs[i].real_sttl) // already forced
                        log_info("admin_state: state of '%s' re-forced from %s to %s, real state is %s", smgrs[i].desc, logf_sttl(smgr_sttl[i]), logf_sttl(updates[i]), smgrs[i].type ? logf_sttl(smgrs[i].real_sttl) : "NA");
                    else
                        log_info("admin_state: state of '%s' forced to %s, real state is %s", smgrs[i].desc, logf_sttl(updates[i]), smgrs[i].type ? logf_sttl(smgrs[i].real_sttl) : "NA");
                    smgr_sttl[i] = updates[i];
                    affected = true;
                }
            } else if (smgr_sttl[i] & GDNSD_STTL_FORCED) { // was forced before, isn't now
                log_info("admin_state: state of '%s' no longer forced (was forced to %s), real and current state is %s", smgrs[i].desc, logf_sttl(smgr_sttl[i]), smgrs[i].type ? logf_sttl(smgrs[i].real_sttl) : "NA");
                smgr_sttl[i] = smgrs[i].real_sttl;
                gdnsd_assert(!(smgr_sttl[i] & GDNSD_STTL_FORCED));
                affected = true;
            }
        }

        if (affected) {
            if (!initial_round)
                kick_sttl_update_timer();
            log_info("admin_state: load complete");
        } else {
            log_info("admin_state: load complete, no net changes");
        }
    }

    free(updates);

    return true;
}

F_NONNULL
static bool admin_process_file(const char* pathname, const bool check_only)
{
    if (check_only)
        log_info("admin_state: checking state file '%s'...", pathname);
    else
        log_info("admin_state: (re-)loading state file '%s'...", pathname);

    bool success = false;

    vscf_data_t* raw = vscf_scan_filename(pathname);
    if (!raw) {
        log_err("admin_state: Loading file '%s' failed", pathname);
    } else {
        if (!vscf_is_hash(raw))
            log_err("admin_state: top level of file '%s' must be a hash", pathname);
        else
            success = admin_process_hash(raw, check_only);
        vscf_destroy(raw);
    }

    if (!success && !check_only)
        log_err("admin_state: file '%s' had errors; all contents were ignored and any current forced states are unaffected", pathname);

    return success;
}

F_NONNULL
static void admin_deleted_file(const char* pathname)
{
    log_info("admin_state: state file '%s' deleted, clearing any forced states...", pathname);
    bool affected = false;
    for (unsigned i = 0; i < num_smgrs; i++) {
        if (smgr_sttl[i] & GDNSD_STTL_FORCED) {
            log_info("admin_state: state of '%s' no longer forced (was forced to %s), real and current state is %s", smgrs[i].desc, logf_sttl(smgr_sttl[i]), smgrs[i].type ? logf_sttl(smgrs[i].real_sttl) : "NA");
            smgr_sttl[i] = smgrs[i].real_sttl;
            gdnsd_assert(!(smgr_sttl[i] & GDNSD_STTL_FORCED));
            affected = true;
        }
    }
    if (affected)
        kick_sttl_update_timer();
}

F_NONNULL
static void admin_timer_cb(struct ev_loop* loop, ev_timer* w, int revents V_UNUSED)
{
    gdnsd_assert(revents == EV_TIMER);
    ev_timer_stop(loop, w);
    ev_stat* afw = &admin_file_watcher;
    if (afw->attr.st_nlink)
        admin_process_file(afw->path, false);
    else
        admin_deleted_file(afw->path);
}

F_NONNULL
static void admin_file_cb(struct ev_loop* loop, ev_stat* w V_UNUSED, int revents V_UNUSED)
{
    gdnsd_assert(revents == EV_STAT);
    ev_timer* aqt = &admin_quiesce_timer;
    if (testsuite_nodelay)
        admin_timer_cb(loop, aqt, EV_TIMER);
    else
        ev_timer_again(loop, aqt);
}

// Note this invoked *after* the initial round of monitoring,
//   but before the main loop begins runtime execution.
F_NONNULL
static void admin_init(struct ev_loop* mloop)
{
    char* pathname = gdnsd_resolve_path_state("admin_state", NULL);

    ev_timer* aqt = &admin_quiesce_timer;
    ev_timer_init(aqt, admin_timer_cb, 0.0, 1.02);
    ev_stat* afw = &admin_file_watcher;
    ev_stat_init(afw, admin_file_cb, pathname, testsuite_nodelay ? 0.01 : 3.0);
    ev_stat_start(mloop, afw);

    // ev_stat_start stat()'s the file for ->attr, use that
    //   to process the file initially if it exists.
    if (afw->attr.st_nlink)
        admin_process_file(pathname, false);
    else
        log_info("admin_state: state file '%s' does not yet exist at startup", pathname);
}

//--------------------------------------------------
// core monitoring stuff
//--------------------------------------------------

// public interface to just check admin_state parsing
void gdnsd_mon_check_admin_file(void)
{
    struct stat st;
    char* pathname = gdnsd_resolve_path_state("admin_state", NULL);

    if (!stat(pathname, &st)) {
        if (!admin_process_file(pathname, true))
            log_fatal("%s has errors!", pathname);
    } else if (errno != ENOENT) {
        log_fatal("Error checking admin_state pathname '%s': %s",
                  pathname, logf_errno());
    }

    free(pathname);
}

// from JSON states output stuff below
static void init_max_states_len(void);

// Called once after all servicetypes and monitored stuff
//  have been configured, from main thread.  mloop happens
//  to be the default loop currently, and should be empty of
//  events at this point so that we can fall out after the
//  initial round of monitoring.
void gdnsd_mon_start(struct ev_loop* mloop)
{
    init_max_states_len();

    // Fall out quickly if nothing to monitor
    if (!num_smgrs)
        return;

    if (getenv("GDNSD_TESTSUITE_NODELAY"))
        testsuite_nodelay = true;

    // saved for timer usage later
    mon_loop = mloop;

    // Run the loop once until all events drain, which will
    // be one full monitoring cycle of each resource (without
    // any artificial delays).
    log_info("Starting initial round of monitoring ...");
    initial_round = true;
    gdnsd_plugins_action_init_monitors(mloop);
    ev_run(mloop, 0);
    log_info("Initial round of monitoring complete");

    // initialize admin_state stuff
    admin_init(mloop);

    // this flag prevents table update timers for admin_init stuff as well!
    initial_round = false;

    // set up the table-update coalescing timer
    ev_timer* sut = &sttl_update_timer;
    ev_timer_init(sut, sttl_table_update, 1.0, 0.0);

    // trigger it once manually to invoke rcu stuff
    //   for the initial round results to ensure there's
    //   no confusion.
    sttl_table_update(mloop, sut, EV_TIMER);

    // add real watchers to the monitor loop for runtime
    //   (the loop itself begins execution later back in main.c)
    gdnsd_plugins_action_start_monitors(mloop);
}

// We only have to check the address, because the port
//  is determined by service type.
F_NONNULL
static bool addr_eq(const gdnsd_anysin_t* a, const gdnsd_anysin_t* b)
{
    gdnsd_assert(a->sa.sa_family == AF_INET || a->sa.sa_family == AF_INET6);

    bool rv = false;
    if (a->sa.sa_family == b->sa.sa_family) {
        if (a->sa.sa_family == AF_INET)
            rv = (a->sin4.sin_addr.s_addr == b->sin4.sin_addr.s_addr);
        else
            rv = !memcmp(a->sin6.sin6_addr.s6_addr, b->sin6.sin6_addr.s6_addr, 16);
    }
    return rv;
}

F_NONNULLX(1)
static unsigned mon_thing(const char* svctype_name, const gdnsd_anysin_t* addr, const char* cname, const uint8_t* dname)
{
    if (addr)
        gdnsd_assert(!cname && !dname);
    else
        gdnsd_assert(cname && dname);

    // first, sort out what svctype_name actually means to us
    service_type_t* this_svc = NULL;
    for (unsigned i = 0; i < num_svc_types; i++) {
        if (!strcmp(svctype_name, service_types[i].name)) {
            this_svc = &service_types[i];
            break;
        }
    }

    if (!this_svc)
        log_fatal("Invalid service type '%s' in monitoring request for '%s'",
                  svctype_name, addr ? logf_anysin_noport(addr) : cname);

    // next, check if this is a duplicate of a request issued earlier
    //   by some other plugin/resource, in which case we can just give
    //   them the existing index
    if (addr) {
        for (unsigned i = 0; i < num_smgrs; i++) {
            smgr_t* that_smgr = &smgrs[i];
            if (!that_smgr->is_cname && addr_eq(addr, &that_smgr->addr) && this_svc == that_smgr->type)
                return i;
        }
    } else {
        for (unsigned i = 0; i < num_smgrs; i++) {
            smgr_t* that_smgr = &smgrs[i];
            if (that_smgr->is_cname && !gdnsd_dname_cmp(dname, that_smgr->dname) && this_svc == that_smgr->type)
                return i;
        }
    }

    // allocate the new smgr/sttl
    const unsigned idx = num_smgrs;
    num_smgrs++;
    smgrs = xrealloc_n(smgrs, num_smgrs, sizeof(*smgrs));
    smgr_t* this_smgr = &smgrs[idx];
    this_smgr->type = this_svc;

    // for a new stype+addr combo, check that the plugin supports addr monitoring
    if (addr) {
        if (this_svc->plugin && !this_svc->plugin->add_mon_addr)
            log_fatal("Service type '%s' does not support address monitoring for '%s'",
                      svctype_name, logf_anysin_noport(addr));

        // construct desc for this new unique monitor
        char addr_str[GDNSD_ANYSIN_MAXSTR];
        int name_err = gdnsd_anysin2str_noport(addr, addr_str);
        // this should basically never happen since the same family of functions will
        //   have already converted it from gdnsd_anysin_t -> text earlier, but if it does,
        //   we really don't have much we can do about logging it informatively...
        if (name_err)
            log_fatal("Error converting address back to text form: %s", gai_strerror(errno));

        this_smgr->desc = gdnsd_str_combine_n(3, addr_str, "/", svctype_name);
        this_smgr->is_cname = false;
        this_smgr->cname = xstrdup(addr_str);
        gdnsd_downcase_str(this_smgr->cname);
        memcpy(&this_smgr->addr, addr, sizeof(this_smgr->addr));
    } else { // cname
        if (this_svc->plugin && !this_svc->plugin->add_mon_cname)
            log_fatal("Service type '%s' does not support CNAME monitoring for '%s'",
                      svctype_name, cname);
        this_smgr->desc = gdnsd_str_combine_n(3, cname, "/", svctype_name);
        this_smgr->is_cname = true;
        this_smgr->cname = xstrdup(cname);
        gdnsd_downcase_str(this_smgr->cname);
        this_smgr->dname = gdnsd_dname_dup(dname);
    }

    this_smgr->n_failure = 0;
    this_smgr->n_success = 0;
    this_smgr->real_sttl = GDNSD_STTL_TTL_MAX;

    // the "down" special gets a different default than the rest
    if (!strcmp(svctype_name, "down"))
        this_smgr->real_sttl |= GDNSD_STTL_DOWN;

    smgr_sttl = xrealloc_n(smgr_sttl, num_smgrs, sizeof(*smgr_sttl));
    smgr_sttl_consumer_ = xrealloc_n(smgr_sttl_consumer_, num_smgrs, sizeof(*smgr_sttl_consumer_));
    smgr_sttl_consumer_[idx] = smgr_sttl[idx] = this_smgr->real_sttl;

    return idx;
}

// Called from plugins once per monitored service type+IP combination
//  to request monitoring and initialize various data/state.
unsigned gdnsd_mon_addr(const char* svctype_name, const gdnsd_anysin_t* addr)
{
    return mon_thing(svctype_name, addr, NULL, NULL);
}

// As above for CNAMEs
unsigned gdnsd_mon_cname(const char* svctype_name, const char* cname, const uint8_t* dname)
{
    return mon_thing(svctype_name, NULL, cname, dname);
}

// .. for virtual entities (e.g. datacenters), which have no service_type
unsigned gdnsd_mon_admin(const char* desc)
{
    const unsigned idx = num_smgrs;
    num_smgrs++;
    smgrs = xrealloc_n(smgrs, num_smgrs, sizeof(*smgrs));
    smgr_sttl = xrealloc_n(smgr_sttl, num_smgrs, sizeof(*smgr_sttl));
    smgr_sttl_consumer_ = xrealloc_n(smgr_sttl_consumer_, num_smgrs, sizeof(*smgr_sttl_consumer_));
    smgr_t* this_smgr = &smgrs[idx];
    memset(this_smgr, 0, sizeof(*this_smgr));
    this_smgr->desc = xstrdup(desc);
    this_smgr->real_sttl = GDNSD_STTL_TTL_MAX;
    smgr_sttl_consumer_[idx] = smgr_sttl[idx] = this_smgr->real_sttl;
    return idx;
}

#define SVC_OPT_UINT(_hash, _typnam, _loc, _min, _max) \
    do { \
        vscf_data_t* _data = vscf_hash_get_data_byconstkey(_hash, #_loc, true); \
        if (_data) { \
            unsigned long _val; \
            if (!vscf_is_simple(_data) \
            || !vscf_simple_get_as_ulong(_data, &_val)) \
                log_fatal("Service type '%s': option '%s': Value must be a positive integer", _typnam, #_loc); \
            if (_val < _min || _val > _max) \
                log_fatal("Service type '%s': option '%s': Value out of range (%lu, %lu)", _typnam, #_loc, _min, _max); \
            this_svc->_loc = (unsigned) _val; \
        } \
    } while (0)

F_NONNULL
static bool bad_svc_opt(const char* key, unsigned klen V_UNUSED, vscf_data_t* d V_UNUSED, const void* svcname_asvoid)
{
    const char* svcname = svcname_asvoid;
    log_fatal("Service type '%s', bad option '%s'", svcname, key);
}

void gdnsd_mon_cfg_stypes_p1(vscf_data_t* svctypes_cfg)
{
    unsigned num_svc_types_cfg = 0;

    if (svctypes_cfg) {
        if (!vscf_is_hash(svctypes_cfg))
            log_fatal("service_types, if defined, must have a hash value");
        num_svc_types_cfg = vscf_hash_get_len(svctypes_cfg);
    }

    num_svc_types = num_svc_types_cfg + 2; // "up", "down"

    // the last 2 service types are fixed to up and down
    service_types = xcalloc_n(num_svc_types, sizeof(*service_types));
    service_types[num_svc_types - 2].name = "up";
    service_types[num_svc_types - 1].name = "down";

    // if this loop executes at all, svctypes_cfg is defined
    //   (see if () block at top of func, and definition of num_svc_types)
    for (unsigned i = 0; i < num_svc_types_cfg; i++) {
        service_type_t* this_svc = &service_types[i];
        this_svc->name = xstrdup(vscf_hash_get_key_byindex(svctypes_cfg, i, NULL));
        if (!strcmp(this_svc->name, "up") || !strcmp(this_svc->name, "down"))
            log_fatal("Explicit service type name '%s' not allowed", this_svc->name);
        vscf_data_t* svctype_cfg = vscf_hash_get_data_byindex(svctypes_cfg, i);
        if (!vscf_is_hash(svctype_cfg))
            log_fatal("Definition of service type '%s' must be a hash", this_svc->name);
        vscf_data_t* pname_cfg = vscf_hash_get_data_byconstkey(svctype_cfg, "plugin", true);
        if (!pname_cfg)
            log_fatal("Service type '%s': 'plugin' must be defined", this_svc->name);
        if (!vscf_is_simple(pname_cfg) || !vscf_simple_get_len(pname_cfg))
            log_fatal("Service type '%s': 'plugin' must be a string", this_svc->name);
        const char* pname = vscf_simple_get_data(pname_cfg);
        this_svc->plugin = gdnsd_plugin_find(pname);
        if (!this_svc->plugin->add_svctype)
            log_fatal("Service type '%s' references plugin '%s', which does not support service monitoring (lacks add_svctype func)", this_svc->name, pname);
    }
}

void gdnsd_mon_cfg_stypes_p2(vscf_data_t* svctypes_cfg)
{

    // If no plugins actually used any plugin-monitored services, there's
    //   no point in setting up the remainder of this.  At the very least
    //   it lets us skip loading http_status.
    bool need_p2 = false;
    for (unsigned i = 0; i < num_smgrs; i++) {
        if (smgrs[i].type) {
            need_p2 = true;
            break;
        }
    }
    if (!need_p2)
        return;

    gdnsd_assert(num_svc_types > 1); // up, down always exist

    for (unsigned i = 0; i < (num_svc_types - 2); i++) {
        gdnsd_assert(svctypes_cfg);
        service_type_t* this_svc = &service_types[i];

        // assert same ordering as _p1
        gdnsd_assert(!strcmp(this_svc->name, vscf_hash_get_key_byindex(svctypes_cfg, i, NULL)));
        gdnsd_assert(this_svc->plugin);

        vscf_data_t* svctype_cfg = vscf_hash_get_data_byindex(svctypes_cfg, i);
        gdnsd_assert(svctype_cfg);

        this_svc->up_thresh = DEF_UP_THRESH;
        this_svc->ok_thresh = DEF_OK_THRESH;
        this_svc->down_thresh = DEF_DOWN_THRESH;
        this_svc->interval = DEF_INTERVAL;
        SVC_OPT_UINT(svctype_cfg, this_svc->name, up_thresh, 1LU, 65535LU);
        SVC_OPT_UINT(svctype_cfg, this_svc->name, ok_thresh, 1LU, 65535LU);
        SVC_OPT_UINT(svctype_cfg, this_svc->name, down_thresh, 1LU, 65535LU);
        SVC_OPT_UINT(svctype_cfg, this_svc->name, interval, 2LU, 3600LU);
        this_svc->timeout = this_svc->interval >> 1U; // default timeout is half of interval
        SVC_OPT_UINT(svctype_cfg, this_svc->name, timeout, 1LU, 300LU);
        if (this_svc->timeout >= this_svc->interval)
            log_fatal("Service type '%s': timeout must be less than interval)", this_svc->name);

        this_svc->plugin->add_svctype(this_svc->name, svctype_cfg, this_svc->interval, this_svc->timeout);
        vscf_hash_iterate_const(svctype_cfg, true, bad_svc_opt, this_svc->name);
    }

    // dummy config for up+down
    for (unsigned i = (num_svc_types - 2); i < num_svc_types; i++) {
        service_type_t* this_svc = &service_types[i];
        this_svc->plugin = NULL;
        this_svc->up_thresh = DEF_UP_THRESH;
        this_svc->ok_thresh = DEF_OK_THRESH;
        this_svc->down_thresh = DEF_DOWN_THRESH;
        this_svc->interval = DEF_INTERVAL;
        this_svc->timeout = 1;
    }

    // now that we've solved the chicken-and-egg, finish processing
    //   the monitoring requests resolver plugins asked about earlier
    for (unsigned i = 0; i < num_smgrs; i++) {
        smgr_t* this_smgr = &smgrs[i];
        gdnsd_assert(this_smgr);
        if (this_smgr->type && this_smgr->type->plugin) {
            if (this_smgr->is_cname) {
                gdnsd_assert(this_smgr->type->plugin->add_mon_cname);
                this_smgr->type->plugin->add_mon_cname(this_smgr->desc, this_smgr->type->name, this_smgr->cname, i);
            } else {
                gdnsd_assert(this_smgr->type->plugin->add_mon_addr);
                this_smgr->type->plugin->add_mon_addr(this_smgr->desc, this_smgr->type->name, this_smgr->cname, &this_smgr->addr, i);
            }
        }
    }
}

F_NONNULL
static void raw_sttl_update(smgr_t* smgr, unsigned idx, gdnsd_sttl_t new_sttl)
{
    gdnsd_assert(idx < num_smgrs);

    // Note that the updater interfaces from monitoring plugins cannot set
    //  the FORCED bit - only the admin-state interface can do that.
    assert_valid_sttl(new_sttl);
    gdnsd_assert(!(new_sttl & GDNSD_STTL_FORCED));

    if (initial_round) {
        log_info("state of '%s' initialized to %s", smgr->desc, logf_sttl(new_sttl));
        smgr_sttl[idx] = smgr->real_sttl = new_sttl;
        // table update taken care of in gdnsd_mon_start()
        //  after all initial monitors complete
    } else if (new_sttl != smgr->real_sttl) {
        if ((new_sttl & GDNSD_STTL_DOWN) != (smgr->real_sttl & GDNSD_STTL_DOWN)) {
            if (smgr_sttl[idx] & GDNSD_STTL_FORCED)
                log_info("state of '%s' changed from %s to %s,"
                         " effective state remains administratively forced to %s",
                         smgr->desc, logf_sttl(smgr->real_sttl), logf_sttl(new_sttl),
                         logf_sttl(smgr_sttl[idx]));
            else
                log_info("state of '%s' changed from %s to %s",
                         smgr->desc, logf_sttl(smgr->real_sttl), logf_sttl(new_sttl));
        }
        smgr->real_sttl = new_sttl;
        if (new_sttl != smgr_sttl[idx] && !(smgr_sttl[idx] & GDNSD_STTL_FORCED)) {
            smgr_sttl[idx] = new_sttl;
            kick_sttl_update_timer();
        }
    }
}

void gdnsd_mon_sttl_updater(unsigned idx, gdnsd_sttl_t new_sttl)
{
    gdnsd_assert(idx < num_smgrs);
    raw_sttl_update(&smgrs[idx], idx, new_sttl);
}

void gdnsd_mon_state_updater(unsigned idx, const bool latest)
{
    gdnsd_assert(idx < num_smgrs);
    smgr_t* smgr = &smgrs[idx];

    // a bit spammy to leave in all debug builds, but handy at times...
    //log_debug("'%s' new monitor result: %s", smgr->desc, latest ? "OK" : "FAIL");

    bool down;

    // XXX think up a better way to set TTL on initial monitoring round?
    //   may involve a whole new counting system, or at least
    //   a count of rounds_since_start until some period has passed?
    //  The idea would be to serve a shorter TTL until stability has
    //   been demonstrated.  For now, just going with pretending initial
    //   state is stable.
    if (initial_round) {
        gdnsd_assert(!smgr->n_failure);
        gdnsd_assert(!smgr->n_success);
        down = !latest;
    } else {
        // First handle basic up/down state and the counters
        down = smgr->real_sttl & GDNSD_STTL_DOWN;
        if (down) { // Currently DOWN
            if (latest) { // New Success
                smgr->n_success++;
                if (smgr->n_success == smgr->type->up_thresh) {
                    smgr->n_success = 0;
                    smgr->n_failure = 0;
                    down = false;
                }
            } else { // New failure when already down, reset for up_thresh
                smgr->n_success = 0;
            }
        } else { // Currently UP
            if (latest) { // New Success
                // Was UP with some intermittent failure history, but has cleared ok_thresh...
                if (smgr->n_failure) {
                    smgr->n_success++;
                    if (smgr->n_success == smgr->type->ok_thresh) {
                        smgr->n_failure = 0;
                        smgr->n_success = 0;
                    }
                }
            } else { // New Failure
                smgr->n_success = 0;
                smgr->n_failure++;
                if (smgr->n_failure == smgr->type->down_thresh) { // Fail threshold check on failure
                    smgr->n_failure = 0;
                    down = true;
                }
            }
        }
    }

    // calculate new TTL based on counters + interval
    const unsigned count_to_change = down
                                     ? smgr->type->up_thresh - smgr->n_success
                                     : smgr->type->down_thresh - smgr->n_failure;
    gdnsd_sttl_t new_sttl = smgr->type->interval * count_to_change;
    if (new_sttl > GDNSD_STTL_TTL_MAX)
        new_sttl = GDNSD_STTL_TTL_MAX;
    if (down)
        new_sttl |= GDNSD_STTL_DOWN;

    raw_sttl_update(smgr, idx, new_sttl);
}

//--------------------------------------------------
// states code from here to the end
//--------------------------------------------------

static const char json_head[] = "{\n";
static const char json_tmpl[] = "\t\"%s\": {\"state\": \"%s\", \"real_state\": \"%s\"}";
static const char json_sep[] = ",\n";
static const char json_foot[] = "\n}\n";
#define json_head_len (sizeof(json_head) - 1U)
#define json_sep_len (sizeof(json_sep) - 1U)
#define json_tmpl_len (sizeof(json_tmpl) - 7U) // 3x%s
#define json_foot_len (sizeof(json_foot) - 1U)

// Initialize max_states_len for runtime use
static void init_max_states_len(void)
{
    gdnsd_assert(!max_states_len);

    //  Note that json_var_len doesn't include the service name length,
    //    and that 5 is the longest state_txt string "DOWN!", and it
    //    counts one more separator than necc (no trailing commas in json)
    const unsigned json_var_len = json_tmpl_len + (5 * 2) + json_sep_len;
    max_states_len = json_head_len + json_foot_len + (num_smgrs * json_var_len);

    for (unsigned i = 0; i < num_smgrs; i++)
        max_states_len += strlen(smgrs[i].desc);

    max_states_len++; // leave room for trailing pointless sprintf \0, JIC

    max_states_len <<= 1U; // double in case anything above is faulty
}

// !!type -> forced -> down
static const char* state_str_map[2][2][2] = {
    {
        // !type
        {
            // !forced
            "NA", // up
            "NA", // down
        },
        {
            // forced
            "UP!",   // up
            "DOWN!", // down
        },
    },
    {
        // has type
        {
            // !forced
            "UP",   // up
            "DOWN", // down
        },
        {
            // forced
            "UP!",   // up
            "DOWN!", // down
        },
    },
};

F_NONNULL
static void get_state_texts(const unsigned i, const char** cur_state_out, const char** real_state_out)
{
    gdnsd_assert(i < num_smgrs);

    *cur_state_out = state_str_map
                     [!!smgrs[i].type]
                     [!!(smgr_sttl[i] & GDNSD_STTL_FORCED)]
                     [!!(smgr_sttl[i] & GDNSD_STTL_DOWN)];
    *real_state_out = state_str_map
                      [!!smgrs[i].type]
                      [!!(smgrs[i].real_sttl & GDNSD_STTL_FORCED)]
                      [!!(smgrs[i].real_sttl & GDNSD_STTL_DOWN)];
}

char* gdnsd_mon_states_get_json(size_t* len)
{
    gdnsd_assert(max_states_len);

    char* buf = xmalloc(max_states_len);
    char* buf_start = buf;

    memcpy(buf, json_head, json_head_len);
    buf += json_head_len;

    for (unsigned i = 0; i < num_smgrs; i++) {
        if (i) {
            // comma separator before all but first element
            memcpy(buf, json_sep, json_sep_len);
            buf += json_sep_len;
        }
        const char* cur_st;
        const char* real_st;
        get_state_texts(i, &cur_st, &real_st);
        const size_t avail = (size_t)(max_states_len - (size_t)(buf - buf_start));
        const int snp_rv = snprintf(buf, avail, json_tmpl, smgrs[i].desc, cur_st, real_st);
        gdnsd_assert(snp_rv > 0 && (size_t)snp_rv < avail);
        buf += (size_t)snp_rv;
    }

    memcpy(buf, json_foot, json_foot_len);
    buf += json_foot_len;

    gdnsd_assert(buf > buf_start);
    const size_t written = (size_t)(buf - buf_start);
    gdnsd_assert(written < max_states_len);

    *len = written;
    return buf_start;
}
