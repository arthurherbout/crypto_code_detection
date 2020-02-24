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

// Unit test for gdmaps

#include <config.h>
#include "gdmaps_test.h"
#include <tap.h>

// *INDENT-OFF*
static const char cfg[] = QUOTE(
   my_prod_map => {
    datacenters => [ dc01, dc02 ],
    nets => {
     192.0.2.128/25 => [ dc02 ],
     10.0.0.0/8 => dc02,
     1234::/16 => dc01
    }
   }
);
// *INDENT-ON*

gdmaps_t* gdmaps = NULL;

int main(int argc V_UNUSED, char* argv[] V_UNUSED)
{
    gdmaps_test_init(getenv("TEST_CFDIR"));
    plan_tests(LOOKUP_CHECK_NTESTS * 17);
    gdmaps = gdmaps_test_load(cfg);
    gdmaps_test_lookup_check(gdmaps, "my_prod_map", "192.0.2.1", "\1\2", 25);
    gdmaps_test_lookup_check(gdmaps, "my_prod_map", "192.0.0.1", "\1\2", 23);
    gdmaps_test_lookup_check(gdmaps, "my_prod_map", "192.0.15.1", "\1\2", 21);
    gdmaps_test_lookup_check(gdmaps, "my_prod_map", "79.125.18.68", "\1\2", 2);
    gdmaps_test_lookup_check(gdmaps, "my_prod_map", "69.58.186.119", "\1\2", 2);
    gdmaps_test_lookup_check(gdmaps, "my_prod_map", "::69.58.186.119", "\1\2", 98); // v4-compat
    gdmaps_test_lookup_check(gdmaps, "my_prod_map", "::FFFF:69.58.186.119", "\1\2", 98); // v4-mapped
    gdmaps_test_lookup_check(gdmaps, "my_prod_map", "::FFFF:0:69.58.186.119", "\1\2", 98); // SIIT
    gdmaps_test_lookup_check(gdmaps, "my_prod_map", "64:ff9b::69.58.186.119", "\1\2", 98); // WKP
    gdmaps_test_lookup_check(gdmaps, "my_prod_map", "2002:453A:BA77::", "\1\2", 18); // 6to4
    gdmaps_test_lookup_check(gdmaps, "my_prod_map", "2001::BAC5:4588", "\1\2", 98); // Teredo
    gdmaps_test_lookup_check(gdmaps, "my_prod_map", "2600:3c00::f03c:91ff:fe96:6a4f", "\1\2", 6); // native v6
    gdmaps_test_lookup_check(gdmaps, "my_prod_map", "192.0.2.223", "\2", 25);
    gdmaps_test_lookup_check(gdmaps, "my_prod_map", "10.1.2.3", "\2", 8);
    gdmaps_test_lookup_check(gdmaps, "my_prod_map", "::10.1.2.3", "\2", 104);
    gdmaps_test_lookup_check(gdmaps, "my_prod_map", "1234:5678::", "\1", 16);
    gdmaps_test_lookup_check(gdmaps, "my_prod_map", "::FFFF:0:10.255.255.255", "\2", 104); // SIIT
    exit(exit_status());
}
