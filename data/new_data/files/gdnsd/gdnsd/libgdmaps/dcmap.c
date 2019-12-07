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

// gdmaps = GeoIP -> Datacenter Mapping library code

#include <config.h>
#include "dcmap.h"

#include "dclists.h"

#include <gdnsd/alloc.h>
#include <gdnsd/log.h>

#include <stdbool.h>

/***************************************
 * dcmap_t and related methods
 **************************************/

// Exactly one of the following must be true:
//  dclist is non-zero, indicating a direct dclist
//  dcmap is non-null, indicating another level of depth
typedef struct {
    char* name;
    dcmap_t* dcmap;
    uint32_t dclist;
} dcmap_child_t;

struct dcmap {
    // All 3 below are allocated to num_children entries.
    dcmap_child_t* children;
    unsigned def_dclist; // copied from parent if not specced in cfg, required at root
    unsigned num_children;
};

typedef struct {
    dcmap_t* dcmap;
    dclists_t* dclists;
    const char* map_name;
    unsigned child_num;
    unsigned true_depth;
    bool allow_auto;
} dcmap_iter_data;

F_NONNULL
static bool dcmap_new_iter(const char* key, unsigned klen V_UNUSED, vscf_data_t* val, void* data)
{
    dcmap_iter_data* did = data;

    dcmap_child_t* child = &did->dcmap->children[did->child_num];
    child->name = xstrdup(key);
    if (vscf_is_hash(val))
        child->dcmap = dcmap_new(val, did->dclists, did->dcmap->def_dclist, did->true_depth + 1, did->map_name, did->allow_auto);
    else
        child->dclist = dclists_find_or_add_vscf(did->dclists, val, did->map_name, did->allow_auto);

    did->child_num++;

    return true;
}

dcmap_t* dcmap_new(vscf_data_t* map_cfg, dclists_t* dclists, const unsigned parent_def, const unsigned true_depth, const char* map_name, const bool allow_auto)
{
    gdnsd_assert(vscf_is_hash(map_cfg));

    dcmap_t* dcmap = xcalloc(sizeof(*dcmap));
    unsigned nchild = vscf_hash_get_len(map_cfg);

    vscf_data_t* def_cfg = vscf_hash_get_data_byconstkey(map_cfg, "default", true);
    if (def_cfg) {
        if (!true_depth) {
            uint8_t newlist[256];
            bool is_auto = dclists_xlate_vscf(dclists, def_cfg, map_name, newlist, allow_auto);
            if (is_auto) {
                gdnsd_assert(allow_auto);
                dcmap->def_dclist = DCLIST_AUTO;
            } else {
                dcmap->def_dclist = 0;
                dclists_replace_list0(dclists, (uint8_t*)xstrdup((char*)newlist));
            }
        } else {
            dcmap->def_dclist = dclists_find_or_add_vscf(dclists, def_cfg, map_name, allow_auto);
        }
        nchild--; // don't iterate "default" later
    } else {
        if (!true_depth) {
            dcmap->def_dclist = allow_auto ? DCLIST_AUTO : 0;
        } else {
            dcmap->def_dclist = parent_def;
        }
    }

    if (nchild) {
        dcmap->num_children = nchild;
        dcmap->children = xcalloc_n(nchild, sizeof(*dcmap->children));
        dcmap_iter_data did = {
            .child_num = 0,
            .dcmap = dcmap,
            .dclists = dclists,
            .map_name = map_name,
            .true_depth = true_depth,
            .allow_auto = allow_auto
        };
        vscf_hash_iterate(map_cfg, true, dcmap_new_iter, &did);
    }

    return dcmap;
}

// as above, but supports abitrary levels of nesting in the map without regard
//   to any named hierarchy, and without prefetching levels from the lookup source
//   unless the map actually wants to see them.
static uint32_t dcmap_llc_(const dcmap_t* dcmap, dcmap_lookup_cb_t cb, void* data, unsigned level)
{
    // map empty within this level, e.g. "US => {}" or "US => { default => [...] }"
    if (!dcmap->num_children)
        return dcmap->def_dclist;

    // This will potentially execute multiple callbacks to search several
    //   levels deep in the network record for a match, but only once we've
    //   explicitly passed the Country level (so search only happens for
    //   subdivisions and cities).
    char lookup[DCMAP_LOOKUP_MAXLEN];
    do {
        lookup[0] = '\0';
        cb(data, &lookup[0], level++);
        if (!lookup[0])
            break;
        for (unsigned i = 0; i < dcmap->num_children; i++) {
            dcmap_child_t* child = &dcmap->children[i];
            if (!strcasecmp(lookup, child->name)) {
                if (child->dcmap)
                    return dcmap_llc_(child->dcmap, cb, data, level);
                return child->dclist;
            }
        }
    } while (level > 2); // >1 => post-continent, >2 => post-country

    return dcmap->def_dclist;
}

uint32_t dcmap_lookup_loc_callback(const dcmap_t* dcmap, dcmap_lookup_cb_t cb, void* data)
{
    return dcmap_llc_(dcmap, cb, data, 0);
}

void dcmap_destroy(dcmap_t* dcmap)
{
    for (unsigned i = 0; i < dcmap->num_children; i++) {
        dcmap_child_t* child = &dcmap->children[i];
        if (child->name)
            free(child->name);
        if (child->dcmap)
            dcmap_destroy(child->dcmap);
    }
    free(dcmap->children);
    free(dcmap);
}
