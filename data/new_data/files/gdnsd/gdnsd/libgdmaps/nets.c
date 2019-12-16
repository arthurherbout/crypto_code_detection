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
#include "nets.h"

#include <gdnsd/log.h>
#include <gdnsd/net.h>

#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// Check whether the passed network is a subnet
//  of (or the entirety of) any of the "undefined"
//  v4-like subspaces in our ntree databases...

F_NONNULL F_PURE
static bool v6_subnet_of(const uint8_t* check, const unsigned check_mask, const uint8_t* v4, const unsigned v4_mask)
{
    gdnsd_assert(!(v4_mask & 7)); // all v4_mask are whole byte masks

    bool rv = false;

    if (check_mask >= v4_mask)
        rv = !memcmp(check, v4, (v4_mask >> 3));

    return rv;
}

F_NONNULL F_PURE
static bool check_v4_issues(const uint8_t* ipv6, const unsigned mask)
{
    gdnsd_assert(mask < 129);

    return (
               v6_subnet_of(ipv6, mask, start_v4mapped, 96)
               || v6_subnet_of(ipv6, mask, start_siit, 96)
               || v6_subnet_of(ipv6, mask, start_wkp, 96)
               || v6_subnet_of(ipv6, mask, start_teredo, 32)
               || v6_subnet_of(ipv6, mask, start_6to4, 16)
           );
}

// arguably, with at least some of the v4-like spaces we could simply translate and hope to de-dupe,
//   if we upgraded nlist_normalize1 to de-dupe matching dclists instead of failing them
F_NONNULL
static bool nets_parse(vscf_data_t* nets_cfg, dclists_t* dclists, const char* map_name, nlist_t* nl)
{
    const unsigned input_nnets = vscf_hash_get_len(nets_cfg);

    for (unsigned i = 0; i < input_nnets; i++) {
        // convert 192.0.2.0/24 -> anysin_t w/ mask in port field
        unsigned net_str_len = 0;
        const char* net_str_cfg = vscf_hash_get_key_byindex(nets_cfg, i, &net_str_len);
        if (net_str_len >= GDNSD_ANYSIN_MAXSTR) {
            log_err("plguin_geoip: map '%s': nets entry '%s' is too long", map_name, net_str_cfg);
            return true;
        }
        char net_str[GDNSD_ANYSIN_MAXSTR];
        memcpy(net_str, net_str_cfg, net_str_len + 1);

        char* mask_str = strchr(net_str, '/');
        if (!mask_str) {
            log_err("plugin_geoip: map '%s': nets entry '%s' does not parse as addr/mask", map_name, net_str);
            return true;
        }
        *mask_str++ = '\0';
        gdnsd_anysin_t tempsin;
        int addr_err = gdnsd_anysin_getaddrinfo(net_str, mask_str, &tempsin);
        if (addr_err) {
            log_err("plugin_geoip: map '%s': nets entry '%s/%s' does not parse as addr/mask: %s", map_name, net_str, mask_str, gai_strerror(addr_err));
            return true;
        }

        unsigned mask;
        uint8_t ipv6[16];

        // now store the anysin data into net_t
        if (tempsin.sa.sa_family == AF_INET6) {
            mask = ntohs(tempsin.sin6.sin6_port);
            if (mask > 128) {
                log_err("plugin_geoip: map '%s': nets entry '%s/%s': illegal IPv6 mask (>128)", map_name, net_str, mask_str);
                return true;
            }
            memcpy(ipv6, tempsin.sin6.sin6_addr.s6_addr, 16);
            if (check_v4_issues(ipv6, mask)) {
                log_err("plugin_geoip: map '%s': 'nets' entry '%s/%s' covers illegal IPv4-like space, see the documentation for more info", map_name, net_str, mask_str);
                return true;
            }
        } else {
            gdnsd_assert(tempsin.sa.sa_family == AF_INET);
            mask = ntohs(tempsin.sin4.sin_port) + 96U;
            if (mask > 128) {
                log_err("plugin_geoip: map '%s': nets entry '%s/%s': illegal IPv4 mask (>32)", map_name, net_str, mask_str);
                return true;
            }
            memset(ipv6, 0, 16);
            memcpy(&ipv6[12], &tempsin.sin4.sin_addr.s_addr, 4);
        }

        // get dclist integer from rhs
        vscf_data_t* dc_cfg = vscf_hash_get_data_byindex(nets_cfg, i);
        const uint32_t dclist = dclists_find_or_add_vscf(dclists, dc_cfg, map_name, false);
        gdnsd_assert(dclist <= DCLIST_MAX); // auto not allowed here
        nlist_append(nl, ipv6, mask, dclist);
    }

    return false;
}

nlist_t* nets_make_list(vscf_data_t* nets_cfg, dclists_t* dclists, const char* map_name)
{
    nlist_t* nl = nlist_new(map_name, false);

    if (nets_cfg) {
        gdnsd_assert(vscf_is_hash(nets_cfg));
        if (nets_parse(nets_cfg, dclists, map_name, nl)) {
            nlist_destroy(nl);
            nl = NULL;
        }
    }

    if (nl) {
        // This masks out the 5x v4-like spaces that we *never*
        //   lookup directly.  These "NN_UNDEF" dclists will
        //   never be seen by runtime lookups.  The only
        //   reason these exist is so that supernets and
        //   adjacent networks get proper masks.  Otherwise
        //   lookups in these nearby spaces might return
        //   oversized edns-client-subnet masks and cause
        //   the cache to affect lookup of these spaces...
        nlist_append(nl, start_v4mapped, 96, NN_UNDEF);
        nlist_append(nl, start_siit, 96, NN_UNDEF);
        nlist_append(nl, start_wkp, 96, NN_UNDEF);
        nlist_append(nl, start_6to4, 16, NN_UNDEF);
        nlist_append(nl, start_teredo, 32, NN_UNDEF);
        nlist_finish(nl);
    }

    return nl;
}
