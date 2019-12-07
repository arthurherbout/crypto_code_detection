/* Copyright © 2012 Brandon L Black <blblack@gmail.com>
 *
 * This file is part of gdnsd.
 *
 * gdnsd-plugin-geoip is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * gdnsd-plugin-geoip is distributed in the hope that it will be useful,
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
#include <gdmaps.h>
#include "plugins.h"

#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>

static gdmaps_t* gdmaps;

F_NONNULL
static unsigned res_get_mapnum(vscf_data_t* res_cfg, const char* res_name)
{
    // Get 'map' name, convert to gdmaps index
    vscf_data_t* map_cfg = vscf_hash_get_data_byconstkey(res_cfg, "map", true);
    if (!map_cfg)
        log_fatal("plugin_geoip: resource '%s': required key 'map' is missing", res_name);
    if (!vscf_is_simple(map_cfg))
        log_fatal("plugin_geoip: resource '%s': 'map' must be a string", res_name);
    const char* map_name = vscf_simple_get_data(map_cfg);
    const int rv = gdmaps_name2idx(gdmaps, map_name);
    if (rv < 0)
        log_fatal("plugin_geoip: resource '%s': map '%s' does not exist", res_name, map_name);
    return (unsigned)rv;
}

static unsigned map_get_len(const unsigned mapnum)
{
    return gdmaps_get_dc_count(gdmaps, mapnum);
}

static unsigned map_get_dcidx(const unsigned mapnum, const char* dcname)
{
    return gdmaps_dcname2num(gdmaps, mapnum, dcname);
}

F_NONNULL
static bool top_config_hook(vscf_data_t* top_config)
{
    gdnsd_assert(vscf_is_hash(top_config));

    vscf_data_t* maps = vscf_hash_get_data_byconstkey(top_config, "maps", true);
    if (!maps)
        log_fatal("plugin_geoip: config has no 'maps' stanza");
    if (!vscf_is_hash(maps))
        log_fatal("plugin_geoip: 'maps' stanza must be a hash");
    if (!vscf_hash_get_len(maps))
        log_fatal("plugin_geoip: 'maps' stanza must contain one or more maps");

    gdmaps = gdmaps_new(maps, gdnsd_mon_admin);

    bool undef_dc_ok = false;
    vscf_data_t* undef_dc_ok_vscf = vscf_hash_get_data_byconstkey(top_config, "undefined_datacenters_ok", true);
    if (undef_dc_ok_vscf && (!vscf_is_simple(undef_dc_ok_vscf) || !vscf_simple_get_as_bool(undef_dc_ok_vscf, &undef_dc_ok)))
        log_fatal("plugin_geoip: 'undef_dc_ok' must be a boolean value ('true' or 'false')");

    return undef_dc_ok;
}

static void bottom_config_hook(void)
{
    gdnsd_assert(gdmaps);
    gdmaps_load_databases(gdmaps);
}

static void plugin_geoip_pre_run(void)
{
    gdnsd_assert(gdmaps);
    gdmaps_setup_watchers(gdmaps);
}

F_NONNULL
static const uint8_t* map_get_dclist(const unsigned mapnum, const client_info_t* cinfo, unsigned* scope_out)
{
    gdnsd_assert(gdmaps);
    return gdmaps_lookup(gdmaps, mapnum, cinfo, scope_out);
}

static unsigned map_get_mon_idx(const unsigned mapnum, const unsigned dcnum)
{
    return gdmaps_map_mon_idx(gdmaps, mapnum, dcnum);
}

#define PNSTR "geoip"
#define CB_LOAD_CONFIG plugin_geoip_load_config
#define CB_MAP plugin_geoip_map_res
#define CB_RES plugin_geoip_resolve
#define META_MAP_ADMIN 1
#include "meta_core.inc"

plugin_t plugin_geoip_funcs = {
    .name = "geoip",
    .config_loaded = false,
    .used = false,
    .load_config = plugin_geoip_load_config,
    .map_res = plugin_geoip_map_res,
    .pre_run = plugin_geoip_pre_run,
    .iothread_init = NULL,
    .iothread_cleanup = NULL,
    .resolve = plugin_geoip_resolve,
    .add_svctype = NULL,
    .add_mon_addr = NULL,
    .add_mon_cname = NULL,
    .init_monitors = NULL,
    .start_monitors = NULL,
};
