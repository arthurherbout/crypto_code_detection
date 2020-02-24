/**********************************************************************
 * Copyright (c) 2015 Jonas Schnelli                                  *
 * Distributed under the MIT software license, see the accompanying   *
 * file COPYING or http://www.opensource.org/licenses/mit-license.php.*
 **********************************************************************/

#if defined HAVE_CONFIG_H
#include "liblogdb-config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "utest.h"

extern void test_sha_256();
extern void test_sha_512();
extern void test_sha_hmac();
extern void test_cstr();
extern void test_utils();
extern void test_serialize();
extern void test_red_black_tree();
extern void test_logdb_memdb();
extern void test_logdb_rbtree();

int U_TESTS_RUN = 0;
int U_TESTS_FAIL = 0;

int main()
{
    u_run_test(test_sha_256);
    u_run_test(test_sha_512);
    u_run_test(test_sha_hmac);
    u_run_test(test_utils);
    u_run_test(test_cstr);
    u_run_test(test_serialize);
    u_run_test(test_red_black_tree);
    u_run_test(test_logdb_memdb);
    u_run_test(test_logdb_rbtree);
    
    return 0;
}
