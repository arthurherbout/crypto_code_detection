/* Copyright © 2014 Brandon L Black <blblack@gmail.com>
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

// Unit test for gdmaps

#include <config.h>
#include "gdmaps_test.h"
#include <tap.h>

// *INDENT-OFF*
static const char cfg[] = QUOTE(
   my_prod_map => {
    geoip2_db => GeoLite2-Country-20141008.mmdb,
    datacenters => [ dc01, dc02 ],
    map => {
     NA => [ dc02, dc01 ],
     EU => { IE => [ dc01 ] },
    }
    nets => extnets.nets
   }
);
// *INDENT-ON*

gdmaps_t* gdmaps = NULL;

int main(int argc V_UNUSED, char* argv[] V_UNUSED)
{
    gdmaps_test_init(getenv("TEST_CFDIR"));
#ifndef HAVE_GEOIP2
    plan_skip_all("No GeoIP2 support");
    exit(exit_status());
#endif
    if (!gdmaps_test_db_exists("GeoLite2-Country-20141008.mmdb")) {
        plan_skip_all("Missing database");
        exit(exit_status());
    }
    plan_tests(LOOKUP_CHECK_NTESTS * 25);
    gdmaps = gdmaps_test_load(cfg);
    gdmaps_test_lookup_check(gdmaps, "my_prod_map", "192.0.2.1", "\1\2", 25);
    gdmaps_test_lookup_check(gdmaps, "my_prod_map", "79.125.18.68", "\1", 17);
    gdmaps_test_lookup_check(gdmaps, "my_prod_map", "69.58.186.119", "\2\1", 17);
    gdmaps_test_lookup_check(gdmaps, "my_prod_map", "::69.58.186.119", "\2\1", 113); // v4-compat
    gdmaps_test_lookup_check(gdmaps, "my_prod_map", "::FFFF:69.58.186.119", "\2\1", 113); // v4-mapped
    gdmaps_test_lookup_check(gdmaps, "my_prod_map", "::FFFF:0:69.58.186.119", "\2\1", 113); // SIIT
    gdmaps_test_lookup_check(gdmaps, "my_prod_map", "64:ff9b::69.58.186.119", "\2\1", 113); // WKP
    gdmaps_test_lookup_check(gdmaps, "my_prod_map", "2002:453A:BA77::", "\2\1", 33); // 6to4
    gdmaps_test_lookup_check(gdmaps, "my_prod_map", "2001::BAC5:4588", "\2\1", 113); // Teredo
    gdmaps_test_lookup_check(gdmaps, "my_prod_map", "2600:3c00::f03c:91ff:fe96:6a4f", "\2\1", 31);
    gdmaps_test_lookup_check(gdmaps, "my_prod_map", "192.0.2.223", "\2", 25);
    gdmaps_test_lookup_check(gdmaps, "my_prod_map", "10.1.2.3", "\2", 16);
    gdmaps_test_lookup_check(gdmaps, "my_prod_map", "::10.1.2.3", "\2", 112);
    gdmaps_test_lookup_check(gdmaps, "my_prod_map", "2222::1", "\2", 20);
    gdmaps_test_lookup_check(gdmaps, "my_prod_map", "10.0.10.5", "\1", 24);
    gdmaps_test_lookup_check(gdmaps, "my_prod_map", "2222:1111::1", "\1", 32);
    // edge-cases on ipv6 nets entry boundaries (subnet/supernet both from "nets")
    gdmaps_test_lookup_check(gdmaps, "my_prod_map", "2222:1110:FFFF:FFFF:FFFF:FFFF:FFFF:FFFF", "\2", 32);
    gdmaps_test_lookup_check(gdmaps, "my_prod_map", "2222:1111::0", "\1", 32);
    gdmaps_test_lookup_check(gdmaps, "my_prod_map", "2222:1111:FFFF:FFFF:FFFF:FFFF:FFFF:FFFF", "\1", 32);
    gdmaps_test_lookup_check(gdmaps, "my_prod_map", "2222:1112::0", "\2", 31);
    // edge-cases on ipv6 nets that merge into existing GeoIP nets
    gdmaps_test_lookup_check(gdmaps, "my_prod_map", "2600:3c00::0", "\2\1", 31);
    gdmaps_test_lookup_check(gdmaps, "my_prod_map", "2600:3c01:FFFF:FFFF:FFFF:FFFF:FFFF:FFFF", "\2\1", 31);
    gdmaps_test_lookup_check(gdmaps, "my_prod_map", "2600:3c02::0", "\2", 32);
    gdmaps_test_lookup_check(gdmaps, "my_prod_map", "2600:3c02:FFFF:FFFF:FFFF:FFFF:FFFF:FFFF", "\2", 32);
    gdmaps_test_lookup_check(gdmaps, "my_prod_map", "2600:3c03::0", "\2\1", 32);
    exit(exit_status());
}
