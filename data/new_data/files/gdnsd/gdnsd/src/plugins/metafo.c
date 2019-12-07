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

#include <gdnsd/compiler.h>
#include <gdnsd/alloc.h>
#include <gdnsd/log.h>
#include <gdnsd/vscf.h>
#include "mon.h"
#include "plugapi.h"
#include "plugins.h"

#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>

typedef struct {
    unsigned num_dcs; // count (e.g. 3)
    uint8_t* dc_list; // 1-based numeric indices (e.g. "\1\2\3\0"), this is always ordered in the metafo case
    char** dc_names;  // array of names, 1-based
} dclist_t;

static unsigned num_dclists = 0;
static dclist_t** dclists = NULL; // one per resource in metafo case

F_NONNULL
static unsigned res_get_mapnum(vscf_data_t* res_cfg, const char* res_name)
{
    // Get 'dclist' name, convert, store, return 0-based dclist index
    vscf_data_t* dc_cfg = vscf_hash_get_data_byconstkey(res_cfg, "datacenters", true);
    if (!dc_cfg)
        log_fatal("plugin_metafo: resource '%s': required key 'datacenters' is missing", res_name);
    dclist_t* dcl = xmalloc(sizeof(*dcl));
    if (vscf_is_hash(dc_cfg))
        log_fatal("plugin_metafo: resource '%s': 'datacenters' value cannot be a hash", res_name);
    dcl->num_dcs = vscf_array_get_len(dc_cfg);
    if (!dcl->num_dcs)
        log_fatal("plugin_metafo: resource '%s': 'datacenters' value cannot be an empty array", res_name);

    uint8_t* dclptr = xmalloc(dcl->num_dcs + 1);
    dcl->dc_list = dclptr;
    dcl->dc_names = xmalloc_n(dcl->num_dcs + 1, sizeof(*dcl->dc_names));
    dcl->dc_names[0] = NULL; // index zero is invalid
    for (unsigned i = 0; i < dcl->num_dcs; i++) {
        vscf_data_t* dcname_cfg = vscf_array_get_data(dc_cfg, i);
        gdnsd_assert(dcname_cfg);
        if (!vscf_is_simple(dcname_cfg))
            log_fatal("plugin_metafo: resource '%s': 'datacenters' must be an array of one or more datacenter name strings", res_name);
        const unsigned dcidx = i + 1;
        *dclptr++ = dcidx;
        dcl->dc_names[dcidx] = xstrdup(vscf_simple_get_data(dcname_cfg));
    }
    *dclptr = 0;

    const unsigned rv_idx = num_dclists;
    num_dclists++;
    dclists = xrealloc_n(dclists, num_dclists, sizeof(*dclists));
    dclists[rv_idx] = dcl;
    return rv_idx;
}

static unsigned map_get_len(const unsigned mapnum)
{
    gdnsd_assert(mapnum < num_dclists);
    gdnsd_assert(dclists[mapnum]->num_dcs);
    return dclists[mapnum]->num_dcs;
}

F_NONNULL F_PURE
static unsigned map_get_dcidx(const unsigned mapnum, const char* dcname)
{
    gdnsd_assert(mapnum < num_dclists);

    dclist_t* this_map = dclists[mapnum];
    for (unsigned i = 1; i <= this_map->num_dcs; i++)
        if (!strcmp(dcname, this_map->dc_names[i]))
            return i;

    return 0;
}

F_NONNULL
static bool top_config_hook(vscf_data_t* top_config V_UNUSED)
{
    gdnsd_assert(vscf_is_hash(top_config));
    return false;
}

static void bottom_config_hook(void)
{
    // metafo doesn't use this, the hook is only useful to geoip
}

F_NONNULL
static const uint8_t* map_get_dclist(const unsigned mapnum, const client_info_t* cinfo V_UNUSED, unsigned* scope_out V_UNUSED)
{
    gdnsd_assert(mapnum < num_dclists);
    return dclists[mapnum]->dc_list;
}

#define PNSTR "metafo"
#define CB_LOAD_CONFIG plugin_metafo_load_config
#define CB_MAP plugin_metafo_map_res
#define CB_RES plugin_metafo_resolve
#define META_MAP_ADMIN 0
#include "meta_core.inc"

plugin_t plugin_metafo_funcs = {
    .name = "metafo",
    .config_loaded = false,
    .used = false,
    .load_config = plugin_metafo_load_config,
    .map_res = plugin_metafo_map_res,
    .pre_run = NULL,
    .iothread_init = NULL,
    .iothread_cleanup = NULL,
    .resolve = plugin_metafo_resolve,
    .add_svctype = NULL,
    .add_mon_addr = NULL,
    .add_mon_cname = NULL,
    .init_monitors = NULL,
    .start_monitors = NULL,
};
