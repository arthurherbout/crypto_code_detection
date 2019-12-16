/* Copyright © 2014 Anton Tolchanov <anton.tolchanov@gmail.com>,
 * Brandon L Black <blblack@gmail.com>, and Jay Reitz <jreitz@gmail.com>
 *
 * This file is part of gdnsd.
 *
 * gdnsd-plugin-weighted is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * gdnsd-plugin-weighted is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with gdnsd.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <config.h>

#include <gdnsd/compiler.h>
#include <gdnsd/alloc.h>
#include <gdnsd/log.h>
#include <gdnsd/vscf.h>
#include <gdnsd/rand.h>
#include "mon.h"
#include "plugapi.h"
#include "plugins.h"

#include <inttypes.h>
#include <stdbool.h>

static const char DEFAULT_SVCNAME[] = "up";

// Importantly, 1048575 * 64 * 64 barely fits in uint32_t
#define MAX_ITEMS_PER_SET 64U
#define MAX_ADDRS_PER_GROUP 64U
#define MAX_WEIGHT 1048575U
#define MAX_WEIGHT_STR "1048575"

/*
  Technically, we could further reduce the 64-limits to just:
   64 addrs per res in ungrouped multi-mode
   64 groups per res in grouped multi-mode
   64 addrs per group in grouped single-mode
  Which frees up situations such as:
   >64 cnames per res
   >64 addrs per res in ungrouped single-mode
   >64 groups per res in grouped single-mode
   >64 addrs per group in grouped multi-mode
  Would still need accurate dynamic array sizing in resolve_dynaddr,
   and would still need to double-check this doesn't cause
   total per-resource weights in excess of 2^32-1 or we
   overflow res->weight (and ditto inside groups in single-mode).
  Will wait for someone to complain about the limits first, though...
*/

typedef struct {
    gdnsd_anysin_t addr;
    unsigned weight;
    unsigned* indices;
} addrstate_t;

typedef struct {
    addrstate_t* as;
    unsigned count;
    unsigned weight;
    unsigned max_weight;
} res_aitem_t;

typedef enum {
    RES_ASET_UNKNOWN = 0,
    RES_ASET_UNGROUPED = 1,
    RES_ASET_GROUPED = 2,
} res_aset_mode_t;

typedef struct {
    res_aitem_t* items;
    char** svc_names;
    unsigned count;
    unsigned max_addrs_pergroup;
    unsigned weight;
    unsigned up_weight;
    unsigned max_weight;
    unsigned num_svcs;
    res_aset_mode_t gmode;
    bool multi;
} addrset_t;

typedef struct {
    uint8_t* cname;
    unsigned weight;
    unsigned* indices;
} res_citem_t;

typedef struct {
    res_citem_t* items;
    char** svc_names;
    unsigned count;
    unsigned weight;
    unsigned up_weight;
    unsigned num_svcs;
} cnset_t;

typedef struct {
    const char* name;
    cnset_t* cnames;
    addrset_t* addrs_v4;
    addrset_t* addrs_v6;
} resource_t;

static resource_t* resources = NULL;
static unsigned num_resources = 0;

// Per-thread PRNGs
static __thread gdnsd_rstate32_t rstate;

static void init_rand(void)
{
    gdnsd_rand32_init(&rstate);
}

static uint64_t get_rand(const uint64_t modval)
{
    gdnsd_assert(modval);
    return gdnsd_rand32_bounded(&rstate, modval);
}

// This is a per-thread 2D array of unsigneds which is indexed like:
//   dyn_addr_weights[item][addr]
// It's used to store temporary outputs while calculating our response during
// runtime lookups in resolve().  It would be a stack buffer like others near
// the top of resolve(), but:
// (1) the size of this buffer ends up being ~16KB even though few of those
//     bytes are commonly used, which seems too high to toss on the stack.
// (2) malloc during resolve() is undesirable in perf terms.
// The values set here are either the configured weight or zero, depending on
// the runtime-variable monitored status of the items in question.
static __thread unsigned(*dyn_addr_weights)[MAX_ADDRS_PER_GROUP] = NULL;

static void init_dyn_addr_weights(void)
{
    dyn_addr_weights = xmalloc_n(MAX_ITEMS_PER_SET, sizeof(*dyn_addr_weights));
}

static void clear_dyn_addr_weights(void)
{
    memset(dyn_addr_weights, 0, sizeof(*dyn_addr_weights) * MAX_ITEMS_PER_SET);
}

// Main config code starts here

F_NONNULL
static void config_item_addrs(res_aitem_t* res_item, const char* res_name, const char* stanza, const char* item_name, const bool ipv6, vscf_data_t* cfg_data, addrset_t* addrset)
{
    long wtemp = 0;
    if (!vscf_is_array(cfg_data)
            || (2 != vscf_array_get_len(cfg_data))
            || !vscf_is_simple(vscf_array_get_data(cfg_data, 0))
            || !vscf_is_simple(vscf_array_get_data(cfg_data, 1))
            || !vscf_simple_get_as_long(vscf_array_get_data(cfg_data, 1), &wtemp)
            || wtemp < 1 || wtemp > MAX_WEIGHT)
        log_fatal("plugin_weighted: resource '%s' (%s): item '%s': values in addrs mode must be arrays of [ IPADDR, WEIGHT ], where weight must be an integer in the range 1 - " MAX_WEIGHT_STR, res_name, stanza, item_name);

    res_item->count = 1;
    res_item->as = xcalloc_n(res_item->count, sizeof(*res_item->as));
    res_item->as[0].weight = wtemp;
    res_item->max_weight = wtemp;
    res_item->weight = wtemp;

    const char* addr_txt = vscf_simple_get_data(vscf_array_get_data(cfg_data, 0));
    int addr_err = gdnsd_anysin_getaddrinfo(addr_txt, NULL, &res_item->as[0].addr);
    if (addr_err)
        log_fatal("plugin_weighted: resource '%s' (%s): item '%s': parsing '%s' as an IP address failed: %s", res_name, stanza, item_name, addr_txt, gai_strerror(addr_err));
    if (ipv6 && res_item->as[0].addr.sa.sa_family != AF_INET6)
        log_fatal("plugin_weighted: resource '%s' (%s): item '%s': '%s' is IPv4, was expecting IPv6", res_name, stanza, item_name, addr_txt);
    else if (!ipv6 && res_item->as[0].addr.sa.sa_family != AF_INET)
        log_fatal("plugin_weighted: resource '%s' (%s): item '%s': '%s' is IPv6, was expecting IPv4", res_name, stanza, item_name, addr_txt);

    if (addrset->num_svcs) {
        res_item->as[0].indices = xmalloc_n(addrset->num_svcs, sizeof(*res_item->as[0].indices));
        for (unsigned i = 0; i < addrset->num_svcs; i++)
            res_item->as[0].indices[i] = gdnsd_mon_addr(addrset->svc_names[i], &res_item->as[0].addr);
    }
    log_debug("plugin_weighted: resource '%s' (%s), item '%s': A '%s' added w/ weight %u", res_name, stanza, item_name, addr_txt, res_item->weight);
}

typedef struct {
    addrset_t* addrset;
    res_aitem_t* res_item;
    const char* res_name;
    const char* stanza;
    const char* item_name;
    bool ipv6;
    unsigned lb_idx;
} iaga_t;

F_NONNULL
static bool config_addr_group_addr(const char* lb_name, const unsigned lb_name_len V_UNUSED, vscf_data_t* lb_data, void* iaga_asvoid)
{
    gdnsd_assert(lb_name_len);

    iaga_t* iaga = iaga_asvoid;

    addrset_t* addrset = iaga->addrset;
    res_aitem_t* res_item = iaga->res_item;
    unsigned lb_idx = iaga->lb_idx;
    iaga->lb_idx++;
    const char* res_name = iaga->res_name;
    const char* stanza = iaga->stanza;
    const char* item_name = iaga->item_name;
    const bool ipv6 = iaga->ipv6;

    long lb_weight = 0;
    if (!vscf_is_array(lb_data)
            || (2 != vscf_array_get_len(lb_data))
            || !vscf_is_simple(vscf_array_get_data(lb_data, 0))
            || !vscf_is_simple(vscf_array_get_data(lb_data, 1))
            || !vscf_simple_get_as_long(vscf_array_get_data(lb_data, 1), &lb_weight)
            || lb_weight < 1 || lb_weight > MAX_WEIGHT)
        log_fatal("plugin_weighted: resource '%s', group '%s': values in address group mode must be arrays of [ IPADDR, WEIGHT ], where weight must be an integer in the range 1 - " MAX_WEIGHT_STR, res_name, item_name);

    res_item->as[lb_idx].weight = lb_weight;

    const char* addr_txt = vscf_simple_get_data(vscf_array_get_data(lb_data, 0));
    int addr_err = gdnsd_anysin_getaddrinfo(addr_txt, NULL, &res_item->as[lb_idx].addr);
    if (addr_err)
        log_fatal("plugin_weighted: resource '%s', group '%s', addr '%s': parsing '%s' as an IP address failed: %s", res_name, item_name, lb_name, addr_txt, gai_strerror(addr_err));
    if (ipv6 && res_item->as[lb_idx].addr.sa.sa_family != AF_INET6)
        log_fatal("plugin_weighted: resource '%s' (%s): item '%s': '%s' is IPv4, was expecting IPv6", res_name, stanza, item_name, addr_txt);
    else if (!ipv6 && res_item->as[lb_idx].addr.sa.sa_family != AF_INET)
        log_fatal("plugin_weighted: resource '%s' (%s): item '%s': '%s' is IPv6, was expecting IPv4", res_name, stanza, item_name, addr_txt);

    if (addrset->num_svcs) {
        res_item->as[lb_idx].indices = xmalloc_n(addrset->num_svcs, sizeof(*res_item->as[lb_idx].indices));
        for (unsigned i = 0; i < addrset->num_svcs; i++)
            res_item->as[lb_idx].indices[i] = gdnsd_mon_addr(addrset->svc_names[i], &res_item->as[lb_idx].addr);
    }

    log_debug("plugin_weighted: resource '%s' (%s), item '%s', address %s added with weight %u", res_name, stanza, item_name, addr_txt, res_item->as[lb_idx].weight);

    return true;
}

F_NONNULL
static void config_item_addr_groups(res_aitem_t* res_item, const char* res_name, const char* stanza, const char* item_name, const bool ipv6, vscf_data_t* cfg_data, addrset_t* addrset)
{
    if (!vscf_is_hash(cfg_data))
        log_fatal("plugin_weighted: resource '%s' (%s), group '%s': groups values must be a hashes", res_name, stanza, item_name);

    unsigned num_addrs = vscf_hash_get_len(cfg_data);

    if (!num_addrs)
        log_fatal("plugin_weighted: resource '%s' (%s), group '%s': must contain one or more label => [ IPADDR, WEIGHT ] settings", res_name, stanza, item_name);
    if (num_addrs > MAX_ADDRS_PER_GROUP)
        log_fatal("plugin_weighted: resource '%s' (%s), group '%s': too many addresses (max %u)", res_name, stanza, item_name, MAX_ADDRS_PER_GROUP);

    res_item->count = num_addrs;
    res_item->as = xcalloc_n(num_addrs, sizeof(*res_item->as));

    iaga_t iaga = {
        .addrset = addrset,
        .res_item = res_item,
        .res_name = res_name,
        .stanza = stanza,
        .item_name = item_name,
        .ipv6 = ipv6,
        .lb_idx = 0
    };
    vscf_hash_iterate(cfg_data, true, config_addr_group_addr, &iaga);

    // sum/max the lb weights
    res_item->weight = 0;
    res_item->max_weight = 0;
    for (unsigned i = 0; i < res_item->count; i++) {
        const unsigned awt = res_item->as[i].weight;
        res_item->weight += awt;
        if (res_item->max_weight < awt)
            res_item->max_weight = awt;
    }

    log_debug("plugin_weighted: resource '%s' (%s), group '%s' with %u addresses & weight %u added", res_name, stanza, item_name, num_addrs, res_item->weight);
}

typedef struct {
    unsigned item_idx;
    addrset_t* addrset;
    const char* res_name;
    const char* stanza;
    bool ipv6;
} addr_iter_data_t;

static bool config_addrset_item(const char* item_name, unsigned klen V_UNUSED, vscf_data_t* cfg_data, void* aid_asvoid)
{

    // pull a bunch of data from addr_iter_data_t...
    addr_iter_data_t* addr_iter_data = aid_asvoid;
    const unsigned item_idx = addr_iter_data->item_idx;
    addr_iter_data->item_idx++;
    addrset_t* addrset = addr_iter_data->addrset;
    const char* res_name = addr_iter_data->res_name;
    const char* stanza = addr_iter_data->stanza;
    const bool ipv6 = addr_iter_data->ipv6;

    // autodetect if not set
    if (unlikely(addrset->gmode == RES_ASET_UNKNOWN)) {
        gdnsd_assert(!item_idx); // should get set on first iteration
        if (vscf_is_hash(cfg_data)) {
            addrset->gmode = RES_ASET_GROUPED;
        } else {
            if (!vscf_is_array(cfg_data))
                log_fatal("plugin_weighted: resource '%s' (%s): item data must be hash (grouped mode) or array (ungrouped mode)", res_name, stanza);
            addrset->gmode = RES_ASET_UNGROUPED;
        }
    }

    res_aitem_t* res_item = &addrset->items[item_idx];
    if (addrset->gmode == RES_ASET_UNGROUPED) {
        config_item_addrs(res_item, res_name, stanza, item_name, ipv6, cfg_data, addrset);
    } else {
        gdnsd_assert(addrset->gmode == RES_ASET_GROUPED);
        config_item_addr_groups(res_item, res_name, stanza, item_name, ipv6, cfg_data, addrset);
    }

    return true;
}

F_NONNULL
static void config_addrset(const char* res_name, const char* stanza, const bool ipv6, addrset_t* addrset, vscf_data_t* cfg)
{
    if (!vscf_is_hash(cfg))
        log_fatal("plugin_weighted: resource '%s' stanza '%s' value must be a hash", res_name, stanza);

    vscf_data_t* parent = vscf_get_parent(cfg);

    // inherit down the applicable res-level parameters
    vscf_hash_inherit(parent, cfg, "service_types", true);
    vscf_hash_inherit(parent, cfg, "multi", true);
    vscf_hash_inherit(parent, cfg, "up_thresh", true);

    // Get a starting assumption of our item count
    addrset->count = vscf_hash_get_len(cfg);

    /////// Process the parameters...

    // service_types
    addrset->num_svcs = 0;
    vscf_data_t* res_stypes = vscf_hash_get_data_byconstkey(cfg, "service_types", true);
    if (res_stypes) {
        addrset->count--; // minus one for service_types entry
        addrset->num_svcs = vscf_array_get_len(res_stypes);
        if (addrset->num_svcs) {
            addrset->svc_names = xmalloc_n(addrset->num_svcs, sizeof(*addrset->svc_names));
            for (unsigned i = 0; i < addrset->num_svcs; i++) {
                vscf_data_t* this_svc_cfg = vscf_array_get_data(res_stypes, i);
                if (!vscf_is_simple(this_svc_cfg))
                    log_fatal("plugin_weighted: resource '%s' (%s): service_types values must be strings", res_name, stanza);
                addrset->svc_names[i] = xstrdup(vscf_simple_get_data(this_svc_cfg));
            }
        }
    } else {
        addrset->num_svcs = 1;
        addrset->svc_names = xmalloc(sizeof(*addrset->svc_names));
        addrset->svc_names[0] = xstrdup(DEFAULT_SVCNAME);
    }

    // multi option
    addrset->multi = false;
    vscf_data_t* multi_cfg = vscf_hash_get_data_byconstkey(cfg, "multi", true);
    if (multi_cfg) {
        addrset->count--; // minus one for multi entry
        if (!vscf_is_simple(multi_cfg) || !vscf_simple_get_as_bool(multi_cfg, &addrset->multi))
            log_fatal("plugin_weighted: resource '%s' (%s): 'multi' must be a boolean value ('true' or 'false')", res_name, stanza);
    }

    // up threshold as double
    double up_thresh = 0.5;
    vscf_data_t* thresh_cfg = vscf_hash_get_data_byconstkey(cfg, "up_thresh", true);
    if (thresh_cfg) {
        addrset->count--; // minus one for up_thresh entry
        if (!vscf_is_simple(thresh_cfg) || !vscf_simple_get_as_double(thresh_cfg, &up_thresh)
                || up_thresh <= 0.0 || up_thresh > 1.0)
            log_fatal("plugin_weighted: resource '%s' (%s): 'up_thresh' must be a floating point value in the range (0.0 - 1.0]", res_name, stanza);
    }

    if (addrset->count > MAX_ITEMS_PER_SET)
        log_fatal("plugin_weighted: resource '%s' (%s): number of direct groups or addrs within one family cannot be more than %u", res_name, stanza, MAX_ITEMS_PER_SET);
    if (!addrset->count)
        log_fatal("plugin_weighted: resource '%s' (%s): empty address-family sets not allowed", res_name, stanza);

    addrset->items = xcalloc_n(addrset->count, sizeof(*addrset->items));
    addrset->gmode = RES_ASET_UNKNOWN;
    addr_iter_data_t aid = {
        .item_idx = 0,
        .addrset = addrset,
        .res_name = res_name,
        .stanza = stanza,
        .ipv6 = ipv6
    };
    vscf_hash_iterate(cfg, true, config_addrset_item, &aid);

    addrset->weight = 0;
    addrset->max_weight = 0;
    for (unsigned i = 0; i < addrset->count; i++) {
        const unsigned iwt = addrset->items[i].weight;
        const unsigned num_addrs = addrset->items[i].count;
        gdnsd_assert(iwt);
        gdnsd_assert(addrset->items[i].max_weight);
        addrset->weight += iwt;
        if (addrset->max_weight < iwt)
            addrset->max_weight = iwt;
        if (addrset->max_addrs_pergroup < num_addrs)
            addrset->max_addrs_pergroup = num_addrs;
    }

    gdnsd_assert(addrset->weight);
    gdnsd_assert(addrset->max_weight);

    addrset->up_weight = gdnsd_uscale_ceil(addrset->weight, up_thresh);
    gdnsd_assert(addrset->up_weight);
}

typedef struct {
    cnset_t* cnset;
    const char* res_name;
    const char* stanza;
    unsigned item_idx;
} cname_iter_data_t;

F_NONNULL
static bool config_item_cname(const char* item_name, unsigned klen V_UNUSED, vscf_data_t* cfg_data, void* cid_asvoid)
{
    cname_iter_data_t* cid = cid_asvoid;

    cnset_t* cnset = cid->cnset;
    const char* res_name = cid->res_name;
    const char* stanza = cid->stanza;
    const unsigned item_idx = cid->item_idx;
    cid->item_idx++;
    res_citem_t* res_item = &cnset->items[item_idx];

    long wtemp = 0;
    if (!vscf_is_array(cfg_data)
            || (2 != vscf_array_get_len(cfg_data))
            || !vscf_is_simple(vscf_array_get_data(cfg_data, 0))
            || !vscf_is_simple(vscf_array_get_data(cfg_data, 1))
            || !vscf_simple_get_as_long(vscf_array_get_data(cfg_data, 1), &wtemp)
            || wtemp < 1 || wtemp > MAX_WEIGHT)
        log_fatal("plugin_weighted: resource '%s' (%s), item '%s': values in cname mode must be arrays of [ CNAME, WEIGHT ], where weight must be an integer in the range 1 - " MAX_WEIGHT_STR, res_name, stanza, item_name);
    res_item->weight = wtemp;

    vscf_data_t* cn = vscf_array_get_data(cfg_data, 0);
    const char* cname_txt = vscf_simple_get_data(cn);
    uint8_t* dname = xmalloc(256);
    dname_status_t dnstat = vscf_simple_get_as_dname(cn, dname);
    if (dnstat == DNAME_INVALID)
        log_fatal("plugin_weighted: resource '%s' (%s), item '%s': '%s' is not a legal domainname", res_name, stanza, item_name, cname_txt);
    if (dnstat == DNAME_PARTIAL)
        log_fatal("plugin_weighted: resource '%s' (%s), item '%s': '%s' must be fully qualified (end in dot)", res_name, stanza, item_name, cname_txt);
    gdnsd_assert(dnstat == DNAME_VALID);
    dname = dname_trim(dname);
    res_item->cname = dname;

    if (cnset->num_svcs) {
        res_item->indices = xmalloc_n(cnset->num_svcs, sizeof(*res_item->indices));
        for (unsigned i = 0; i < cnset->num_svcs; i++)
            res_item->indices[i] = gdnsd_mon_cname(cnset->svc_names[i], cname_txt, dname);
    }

    log_debug("plugin_weighted: resource '%s' (%s), item '%s', CNAME '%s' added with weight %u", res_name, stanza, item_name, logf_dname(dname), res_item->weight);

    return true;
}

F_NONNULL
static void config_cnameset(const char* res_name, const char* stanza, cnset_t* cnset, vscf_data_t* cfg)
{
    if (!vscf_is_hash(cfg))
        log_fatal("plugin_weighted: resource '%s' stanza '%s' value must be a hash", res_name, stanza);

    cnset->count = vscf_hash_get_len(cfg);

    // service_types
    cnset->num_svcs = 0;
    vscf_data_t* res_stypes = vscf_hash_get_data_byconstkey(cfg, "service_types", true);
    if (res_stypes) {
        cnset->count--; // minus one for service_types entry
        cnset->num_svcs = vscf_array_get_len(res_stypes);
        if (cnset->num_svcs) {
            cnset->svc_names = xmalloc_n(cnset->num_svcs, sizeof(*cnset->svc_names));
            for (unsigned i = 0; i < cnset->num_svcs; i++) {
                vscf_data_t* this_svc_cfg = vscf_array_get_data(res_stypes, i);
                if (!vscf_is_simple(this_svc_cfg))
                    log_fatal("plugin_weighted: resource '%s' (%s): service_types values must be strings", res_name, stanza);
                cnset->svc_names[i] = xstrdup(vscf_simple_get_data(this_svc_cfg));
            }
        }
    } else {
        cnset->num_svcs = 1;
        cnset->svc_names = xmalloc(sizeof(*cnset->svc_names));
        cnset->svc_names[0] = xstrdup(DEFAULT_SVCNAME);
    }

    // up threshold as double
    double up_thresh = 0.5;
    vscf_data_t* thresh_cfg = vscf_hash_get_data_byconstkey(cfg, "up_thresh", true);
    if (thresh_cfg) {
        cnset->count--; // minus one for up_thresh entry
        if (!vscf_is_simple(thresh_cfg) || !vscf_simple_get_as_double(thresh_cfg, &up_thresh)
                || up_thresh <= 0.0 || up_thresh > 1.0)
            log_fatal("plugin_weighted: resource '%s' (%s): 'up_thresh' must be a floating point value in the range (0.0 - 1.0]", res_name, stanza);
    }

    // multi option is processed for count-correctness, but ignored (it's not legal
    //   here, but may be present due to inheritance of defaults!)
    if (vscf_hash_get_data_byconstkey(cfg, "multi", true))
        cnset->count--;

    if (cnset->count > MAX_ITEMS_PER_SET)
        log_fatal("plugin_weighted: resource '%s' (%s): number of cnames cannot be more than %u", res_name, stanza, MAX_ITEMS_PER_SET);
    if (!cnset->count)
        log_fatal("plugin_weighted: resource '%s' (%s): empty cname sets not allowed", res_name, stanza);

    cnset->items = xcalloc_n(cnset->count, sizeof(*cnset->items));
    cname_iter_data_t cid = {
        .cnset = cnset,
        .res_name = res_name,
        .stanza = stanza,
        .item_idx = 0,
    };
    vscf_hash_iterate(cfg, true, config_item_cname, &cid);

    cnset->weight = 0;
    for (unsigned i = 0; i < cnset->count; i++) {
        const unsigned cwt = cnset->items[i].weight;
        gdnsd_assert(cwt);
        cnset->weight += cwt;
    }

    gdnsd_assert(cnset->weight);

    cnset->up_weight = gdnsd_uscale_ceil(cnset->weight, up_thresh);
}

F_NONNULL
static void config_auto(resource_t* res, vscf_data_t* res_cfg)
{
    gdnsd_assert(vscf_is_hash(res_cfg));

    // mark all possible parameter-keys
    vscf_hash_get_data_byconstkey(res_cfg, "service_types", true);
    vscf_hash_get_data_byconstkey(res_cfg, "multi", true);
    vscf_hash_get_data_byconstkey(res_cfg, "up_thresh", true);

    // make a copy that contains no parameters, only item-name keys
    vscf_data_t* res_cfg_noparams = vscf_clone(res_cfg, true);

    if (!vscf_hash_get_len(res_cfg_noparams))
        log_fatal("plugin_weighted: resource '%s' (direct) contains no weighted items", res->name);

    const char* first_name = vscf_hash_get_key_byindex(res_cfg_noparams, 0, NULL);
    vscf_data_t* first_cfg = vscf_hash_get_data_byindex(res_cfg_noparams, 0);
    if (vscf_is_hash(first_cfg)) { // grouped address mode...
        if (!vscf_hash_get_len(first_cfg))
            log_fatal("plugin_weighted: resource '%s' (direct): group '%s': contains no addresses", res->name, first_name);
        const char* lb_name = vscf_hash_get_key_byindex(first_cfg, 0, NULL);
        vscf_data_t* lb_cfg = vscf_hash_get_data_byindex(first_cfg, 0);
        if (!vscf_is_array(lb_cfg) || !vscf_array_get_len(lb_cfg) || !vscf_is_simple(vscf_array_get_data(lb_cfg, 0)))
            log_fatal("plugin_weighted: resource '%s' (direct): group '%s': item '%s': value must be an array of [ IP, weight ]", res->name, first_name, lb_name);
        const char* first_addr_txt = vscf_simple_get_data(vscf_array_get_data(lb_cfg, 0));
        gdnsd_anysin_t temp_sin;
        int addr_err = gdnsd_anysin_getaddrinfo(first_addr_txt, NULL, &temp_sin);
        if (addr_err)
            log_fatal("plugin_weighted: resource '%s' (direct): group '%s': item '%s': could not parse '%s' as an IP address: %s", res->name, first_name, lb_name, first_addr_txt, gai_strerror(addr_err));
        if (temp_sin.sa.sa_family == AF_INET6) {
            res->addrs_v6 = xcalloc(sizeof(*res->addrs_v6));
            config_addrset(res->name, "direct", true, res->addrs_v6, res_cfg);
        } else {
            gdnsd_assert(temp_sin.sa.sa_family == AF_INET);
            res->addrs_v4 = xcalloc(sizeof(*res->addrs_v4));
            config_addrset(res->name, "direct", false, res->addrs_v4, res_cfg);
        }
    } else if (vscf_is_array(first_cfg)) { // ungrouped address, or cnames
        vscf_data_t* first_ac = vscf_array_get_data(first_cfg, 0);
        if (!first_ac || !vscf_is_simple(first_ac))
            log_fatal("plugin_weighted: resource '%s' (direct): item '%s': first element of array should be an IP address or CNAME string", res->name, first_name);
        gdnsd_anysin_t temp_sin;
        if (gdnsd_anysin_getaddrinfo(vscf_simple_get_data(first_ac), NULL, &temp_sin)) {
            // was not a valid address, try cnames mode
            res->cnames = xcalloc(sizeof(*res->cnames));
            config_cnameset(res->name, "direct", res->cnames, res_cfg);
        } else {
            // was a valid address, try addrset mode
            if (temp_sin.sa.sa_family == AF_INET6) {
                res->addrs_v6 = xcalloc(sizeof(*res->addrs_v6));
                config_addrset(res->name, "direct", true, res->addrs_v6, res_cfg);
            } else {
                gdnsd_assert(temp_sin.sa.sa_family == AF_INET);
                res->addrs_v4 = xcalloc(sizeof(*res->addrs_v4));
                config_addrset(res->name, "direct", false, res->addrs_v4, res_cfg);
            }
        }
    } else {
        log_fatal("plugin_weighted: resource '%s' (direct): item '%s': resource type not detectable (should be array of [ IP, weight ], array of [ CNAME, weight ], or hashed address group ...)", res->name, first_name);
    }

    vscf_destroy(res_cfg_noparams);
}

F_NONNULL
static bool res_mixed_fail(const char* item_name, unsigned klen V_UNUSED, vscf_data_t* d V_UNUSED, const void* rname_asvoid)
{
    const char* rname = rname_asvoid;
    log_fatal("plugin_weighted: resource '%s' seems to have explicit 'addrs_v4', 'addrs_v6', or 'cnames' configuration mixed with direct item config (e.g. '%s'), which is not allowed", rname, item_name);
    return false;
}

static bool config_res(const char* res_name, unsigned klen V_UNUSED, vscf_data_t* res_cfg, void* idx_asvoid)
{
    unsigned* idx_ptr = idx_asvoid;
    resource_t* res = &resources[(*idx_ptr)++];
    res->name = xstrdup(res_name);
    if (!vscf_is_hash(res_cfg))
        log_fatal("plugin_weighted: the value of resource '%s' must be a hash", res_name);

    /* Resource config:
     * resX => {
     *     addrs_v4 => { ... (IPv4, grouped or not)  }
     *     addrs_v6 => { ... (IPv6, grouped or not)  }
     * }
     * OR: auto-detect a set of all-IPv4, all-IPv6, or all-CNAMEs
     */

    // grab explicit sub-stanzas
    vscf_data_t* addrs_v4_cfg = vscf_hash_get_data_byconstkey(res_cfg, "addrs_v4", true);
    vscf_data_t* addrs_v6_cfg = vscf_hash_get_data_byconstkey(res_cfg, "addrs_v6", true);
    vscf_data_t* cnames_cfg = vscf_hash_get_data_byconstkey(res_cfg, "cnames", true);
    if (cnames_cfg)
        log_fatal("plugin_weighted: resource '%s': the pointless singleton 'cnames' substanza is no longer supported; move the data up a level without it", res_name);

    if (addrs_v4_cfg) {
        res->addrs_v4 = xcalloc(sizeof(*res->addrs_v4));
        config_addrset(res_name, "addrs_v4", false, res->addrs_v4, addrs_v4_cfg);
    }

    if (addrs_v6_cfg) {
        res->addrs_v6 = xcalloc(sizeof(*res->addrs_v6));
        config_addrset(res_name, "addrs_v6", true, res->addrs_v6, addrs_v6_cfg);
    }

    if (!addrs_v4_cfg && !addrs_v6_cfg) {
        config_auto(res, res_cfg);
    } else {
        // if unmarked keys remain in res_cfg (explicit sets would have marked params),
        //    fail due to mixed direct cfg + sub-stanzas.
        vscf_hash_iterate_const(res_cfg, true, res_mixed_fail, res_name);
    }

    return true;
}

////// exported callbacks start here

static void plugin_weighted_load_config(vscf_data_t* config, const unsigned num_threads V_UNUSED)
{
    gdnsd_assert(config);
    gdnsd_assert(vscf_is_hash(config));

    num_resources = vscf_hash_get_len(config);
    if (vscf_hash_bequeath_all(config, "service_types", true, false))
        num_resources--; // don't count service_types
    if (vscf_hash_bequeath_all(config, "multi", true, false))
        num_resources--; // don't count multi
    if (vscf_hash_bequeath_all(config, "up_thresh", true, false))
        num_resources--; // don't count up_thresh

    if (num_resources) {
        resources = xcalloc_n(num_resources, sizeof(*resources));
        unsigned idx = 0;
        vscf_hash_iterate(config, true, config_res, &idx);
    }

    // find maximum per-address-family address output counts...
    unsigned max_v4 = 0;
    unsigned max_v6 = 0;
    for (unsigned i = 0; i < num_resources; i++) {
        resource_t* res = &resources[i];
        if (res->addrs_v4) {
            addrset_t* aset = res->addrs_v4;
            const unsigned max = aset->multi
                                 ? aset->count
                                 : aset->max_addrs_pergroup;
            if (max > max_v4)
                max_v4 = max;
        }
        if (res->addrs_v6) {
            addrset_t* aset = res->addrs_v6;
            const unsigned max = aset->multi
                                 ? aset->count
                                 : aset->max_addrs_pergroup;
            if (max > max_v6)
                max_v6 = max;
        }
    }
    gdnsd_dyn_addr_max(max_v4, max_v6);
}

static int plugin_weighted_map_res(const char* resname, const uint8_t* zone_name)
{
    if (!resname)
        map_res_err("plugin_weighted: resource name required");

    for (unsigned i = 0; i < num_resources; i++) {
        if (!strcmp(resname, resources[i].name)) {
            cnset_t* cnset = resources[i].cnames;
            if (cnset) {
                if (!zone_name)
                    map_res_err("plugin_weighted: Resource '%s' used in a DYNA RR, but has CNAME data", resources[i].name);
                for (unsigned j = 0; j < cnset->count; j++) {
                    const uint8_t* dname = cnset->items[j].cname;
                    if (dname_isinzone(zone_name, dname))
                        map_res_err("plugin_weighted: Resource '%s' CNAME value '%s' cannot be used within zone '%s'", resources[i].name, logf_dname(dname), logf_dname(zone_name));
                }
            }
            log_debug("plugin_weighted: resource '%s' mapped", resources[i].name);
            return (int)i;
        }
    }

    map_res_err("plugin_weighted: unknown resource '%s'", resname);
}

static void plugin_weighted_iothread_init(void)
{
    init_rand();
    init_dyn_addr_weights();
}

static void plugin_weighted_iothread_cleanup(void)
{
    free(dyn_addr_weights);
}

F_NONNULL
static gdnsd_sttl_t resolve_cname(const gdnsd_sttl_t* sttl_tbl, const resource_t* resource, dyn_result_t* result)
{
    cnset_t* cnset = resource->cnames;
    gdnsd_assert(cnset);
    gdnsd_assert(cnset->weight);

    gdnsd_sttl_t rv = GDNSD_STTL_TTL_MAX;

    // first, iterate the CNAMEs and build an array of
    //   dynamic weights (0 if down, normal weight if up)
    //   as well as a sum of all dynamic weights
    const unsigned ct = cnset->count;
    unsigned dyn_sum = 0;
    unsigned dyn_weights[MAX_ITEMS_PER_SET];
    for (unsigned i = 0; i < ct; i++) {
        const res_citem_t* citem = &cnset->items[i];
        const gdnsd_sttl_t citem_sttl
            = gdnsd_sttl_min(sttl_tbl, citem->indices, cnset->num_svcs);
        rv = gdnsd_sttl_min2(rv, citem_sttl);
        if (citem_sttl & GDNSD_STTL_DOWN) {
            dyn_weights[i] = 0;
        } else {
            dyn_weights[i] = citem->weight;
            dyn_sum += citem->weight;
        }
    }

    // if the dynamic sum fails the up_thresh check,
    //   redo the above pretending everything is up,
    //   but make sure the retval says DOWN to
    //   upstream callers
    if (dyn_sum < cnset->up_weight) {
        rv |= GDNSD_STTL_DOWN;
        dyn_sum = cnset->weight;
        for (unsigned i = 0; i < ct; i++) {
            const res_citem_t* citem = &cnset->items[i];
            dyn_weights[i] = citem->weight;
        }
    } else {
        // if up_thresh check passed, clear any DOWN flag
        //  which came from an individual CNAME into
        //  our final retval
        rv &= ~GDNSD_STTL_DOWN;
    }

    gdnsd_assert(dyn_sum);

    // choose the first item that breaks the random threshold
    const unsigned item_rand = get_rand(dyn_sum);
    unsigned running_total = 0;
    unsigned chosen = 0;
    for (unsigned i = 0; i < ct; i++) {
        running_total += dyn_weights[i];
        if (item_rand < running_total) {
            chosen = i;
            break;
        }
    }

    // set the output stuff
    gdnsd_result_add_cname(result, cnset->items[chosen].cname);

    return rv;
}

F_NONNULL
static gdnsd_sttl_t resolve(const gdnsd_sttl_t* sttl_tbl, const addrset_t* aset, dyn_result_t* result)
{
    const unsigned num_items = aset->count;
    unsigned dyn_items_sum = 0; // sum of dyn_item_sums[]
    unsigned dyn_items_max = 0; // max of dyn_item_sums[]
    unsigned dyn_item_sums[MAX_ITEMS_PER_SET]; // sum of dyn_addr_weights[N][]
    unsigned dyn_item_maxs[MAX_ITEMS_PER_SET]; // max of dyn_addr_weights[N][]

    clear_dyn_addr_weights();

    gdnsd_sttl_t rv = GDNSD_STTL_TTL_MAX;

    // Get dynamic info about each item
    for (unsigned item_idx = 0; item_idx < num_items; item_idx++) {
        const res_aitem_t* res_item = &aset->items[item_idx];
        dyn_item_sums[item_idx] = 0;
        dyn_item_maxs[item_idx] = 0;
        for (unsigned addr_idx = 0; addr_idx < res_item->count; addr_idx++) {
            const addrstate_t* addr = &res_item->as[addr_idx];
            const gdnsd_sttl_t addr_sttl
                = gdnsd_sttl_min(sttl_tbl, addr->indices, aset->num_svcs);
            rv = gdnsd_sttl_min2(rv, addr_sttl);
            if (!(addr_sttl & GDNSD_STTL_DOWN)) {
                dyn_addr_weights[item_idx][addr_idx] = addr->weight;
                dyn_item_sums[item_idx] += addr->weight;
                if (addr->weight > dyn_item_maxs[item_idx])
                    dyn_item_maxs[item_idx] = addr->weight;
            }
        }
    }

    // summarize dynamic info at the resource level
    for (unsigned item_idx = 0; item_idx < num_items; item_idx++) {
        const unsigned isum = dyn_item_sums[item_idx];
        dyn_items_sum += isum;
        if (dyn_items_max < isum)
            dyn_items_max = isum;
    }

    // if all items looked completely-down, treat them all as completely-up
    if (dyn_items_sum < aset->up_weight) {
        rv |= GDNSD_STTL_DOWN;
        dyn_items_sum = aset->weight;
        dyn_items_max = aset->max_weight;
        for (unsigned item_idx = 0; item_idx < num_items; item_idx++) {
            const res_aitem_t* res_item = &aset->items[item_idx];
            dyn_item_sums[item_idx] = res_item->weight;
            dyn_item_maxs[item_idx] = res_item->max_weight;
            for (unsigned addr_idx = 0; addr_idx < res_item->count; addr_idx++)
                dyn_addr_weights[item_idx][addr_idx] = res_item->as[addr_idx].weight;
        }
    } else {
        rv &= ~GDNSD_STTL_DOWN;
    }

    gdnsd_assert(dyn_items_sum);
    gdnsd_assert(dyn_items_max);

    if (aset->multi) {
        // Outer decision: choose multiple items based on dyn_items_max
        for (unsigned item_idx = 0; item_idx < num_items; item_idx++) {
            const res_aitem_t* res_item = &aset->items[item_idx];
            const unsigned item_rand = get_rand(dyn_items_max);
            const unsigned isum = dyn_item_sums[item_idx];
            if (item_rand < isum) {
                gdnsd_assert(isum); // given that they're both uints
                // Inner decision: choose one addr based on dyn_item->sum
                const unsigned addr_rand = get_rand(isum);
                unsigned addr_running_total = 0;
                for (unsigned addr_idx = 0; addr_idx < res_item->count; addr_idx++) {
                    addr_running_total += dyn_addr_weights[item_idx][addr_idx];
                    if (addr_rand < addr_running_total) {
                        gdnsd_result_add_anysin(result, &res_item->as[addr_idx].addr);
                        break;
                    }
                }
            }
        }
    } else {
        // Outer decision: choose one item based on dyn_items_sum
        const unsigned item_rand = get_rand(dyn_items_sum);
        unsigned item_running_total = 0;
        for (unsigned item_idx = 0; item_idx < num_items; item_idx++) {
            item_running_total += dyn_item_sums[item_idx];
            if (item_rand < item_running_total) {
                const res_aitem_t* chosen = &aset->items[item_idx];
                // Inner decision: choose multiple addrs based on chosen's dynamic max
                const unsigned addr_max = dyn_item_maxs[item_idx];
                gdnsd_assert(addr_max);
                for (unsigned addr_idx = 0; addr_idx < chosen->count; addr_idx++) {
                    const unsigned addr_rand = get_rand(addr_max);
                    if (addr_rand < dyn_addr_weights[item_idx][addr_idx])
                        gdnsd_result_add_anysin(result, &chosen->as[addr_idx].addr);
                }
                break;
            }
        }
    }

    assert_valid_sttl(rv);
    return rv;
}

F_NONNULL
static gdnsd_sttl_t resolve_addr(const gdnsd_sttl_t* sttl_tbl, const resource_t* res, dyn_result_t* result)
{
    gdnsd_sttl_t rv;

    if (res->addrs_v4) {
        rv = resolve(sttl_tbl, res->addrs_v4, result);
        if (res->addrs_v6) {
            const gdnsd_sttl_t v6_rv = resolve(sttl_tbl, res->addrs_v6, result);
            rv = gdnsd_sttl_min2(rv, v6_rv);
        }
    } else {
        gdnsd_assert(res->addrs_v6);
        rv = resolve(sttl_tbl, res->addrs_v6, result);
    }

    assert_valid_sttl(rv);
    return rv;
}

static gdnsd_sttl_t plugin_weighted_resolve(unsigned resnum, const client_info_t* cinfo V_UNUSED, dyn_result_t* result)
{
    const resource_t* resource = &resources[resnum];
    gdnsd_assert(resource);

    gdnsd_sttl_t rv;

    const gdnsd_sttl_t* sttl_tbl = gdnsd_mon_get_sttl_table();

    if (resource->cnames) {
        rv = resolve_cname(sttl_tbl, resource, result);
    } else {
        rv = resolve_addr(sttl_tbl, resource, result);
    }

    assert_valid_sttl(rv);
    return rv;
}

plugin_t plugin_weighted_funcs = {
    .name = "weighted",
    .config_loaded = false,
    .used = false,
    .load_config = plugin_weighted_load_config,
    .map_res = plugin_weighted_map_res,
    .pre_run = NULL,
    .iothread_init = plugin_weighted_iothread_init,
    .iothread_cleanup = plugin_weighted_iothread_cleanup,
    .resolve = plugin_weighted_resolve,
    .add_svctype = NULL,
    .add_mon_addr = NULL,
    .add_mon_cname = NULL,
    .init_monitors = NULL,
    .start_monitors = NULL,
};
