/*
    Itzam/C (version 7.0) is an embedded database engine written in Standard C.
    
    It is part of the Drakontos Library of Interesting and Esoteric Oddities

    Copyright 2016 Scott Robert Ladd. All rights reserved.

    Itzam/C is user-supported open source software. It's continued development is dependent on
    financial support from the community. You can provide funding by visiting the Itzam/C
    website at:

        http://www.drakontos.com

    You license Itzam/C under the Simplified BSD License (FreeBSD License), the text of which 
    is available at the website above. 
*/

#include "../src/itzam.h"
#include "itzam_errors.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

//-- embedded random number generator; ala Park and Miller
static int32_t seed = 1325;

void init_test_prng(int32_t s) {
    seed = s;
}

int32_t random_int32(int32_t limit) {
    static const int32_t IA   = 16807;
    static const int32_t IM   = 2147483647;
    static const int32_t IQ   = 127773;
    static const int32_t IR   = 2836;
    static const int32_t MASK = 123459876;

    int32_t k;
    int32_t result;

    seed ^= MASK;
    k = seed / IQ;
    seed = IA * (seed - k * IQ) - IR * k;

    if (seed < 0L)
        seed += IM;

    result = (seed % limit);
    seed ^= MASK;

    return result;
}

//-- Reports an itzam error
void not_okay(itzam_state state) {
    fprintf(stderr, "\nItzam problem: %s\n", STATE_MESSAGES[state]);
    exit(EXIT_FAILURE);
}

void error_handler(const char * function_name, itzam_error error) {
    fprintf(stderr, "Itzam error in %s: %s\n", function_name, ERROR_STRINGS[error]);
    exit(EXIT_FAILURE);
}

//-- global tracking values

int32_t total_writes  = 0;
int32_t total_finds   = 0;
int32_t total_removes = 0;

//-- Verifies that the database contains the expected records

static bool verify(itzam_btree * btree, bool * key_flags, int maxkey, int32_t rec_size) {
    bool result = true;
    int32_t key;
    int32_t * rec;

    rec = (int32_t *)malloc(rec_size * sizeof(int32_t));

    for (key = 0; key < maxkey; ++key) {
        ++total_finds;
        if (itzam_btree_find(btree,(const void *)(&key),(void *)(rec))) {
            if (key_flags[key]) {
                if (rec[0] != key) {
                    printf("data does not match key %d\n", key);
                    result = false;
                }
            }
            else {
                printf("key %d found, and should not have been\n", key);
                result = false;
            }
        }
        else {
            if (key_flags[key]) {
                printf("expected key %d not found\n", key);
                result = false;
            }
        }
    }

    free(rec);

    return result;
}

//-- tests

bool test_btree_stress() {
    itzam_btree  btree;
    itzam_state  state;
    int32_t      key;
    itzam_int    n, i;
    itzam_int    rec_size         = 64; // times sizeof int32_t (4 byte each)
    int32_t      chance_trans     = 30;
    int32_t      chance_commit    = 50;
    int32_t      chance_rollback  = 60;
    itzam_int    add_count        = 0;
    itzam_int    rem_count        = 0;
    itzam_int    tran_count       = 0;
    itzam_int    roll_count       = 0;
    itzam_int    commit_count     = 0;
    bool         in_transaction   = false;
    bool *       save_flags       = NULL;
    itzam_int    save_add_count   = 0;
    itzam_int    save_rem_count   = 0;
    char *       filename         = "stress.itz";
    itzam_int    maxkey           =  2500000;
    itzam_int    test_size        = 10000000;
    int          order            = 25;
    bool         verified_run     = false;
    bool         use_transactions = false;
    bool *       key_flags        = (bool *)malloc(maxkey * sizeof(bool));

    if (verified_run) {
        maxkey = maxkey / 10000;
        test_size = test_size / 10000;
    }

    time_t start = time(NULL);

    for (n = 0; n < maxkey; ++n)
        key_flags[n] = false;

    // banner for this test
    printf("\nItzam/C B-Tree Test\nRandomized Stress Test\n");

    state = itzam_btree_create(&btree, filename, order, sizeof(int32_t) * rec_size, itzam_comparator_int32, error_handler);

    if (state != ITZAM_OKAY) {
        not_okay(state);
        return false;
    }

    save_flags = (bool *)malloc(sizeof(bool) * maxkey);

    for (n = 1; n <= test_size; ++ n) {
        if (use_transactions && !in_transaction) {
            if (random_int32(100) < chance_trans)  {
                state = itzam_btree_transaction_start(&btree);
                in_transaction = true;

                for (i = 0; i < maxkey; ++i)
                    save_flags[i] = key_flags[i];

                save_add_count = add_count;
                save_rem_count = rem_count;
                ++tran_count;
            }
        }

        key = (int32_t)random_int32((int32_t)maxkey);
        int32_t * record = (int32_t *)malloc(rec_size * sizeof(int32_t));
        record[0] = key; // we only care, for testing, about first element
        state = itzam_btree_insert(&btree,(const void *)record);
        ++total_writes;
        free(record);

        switch (state) {
            case ITZAM_OKAY:
                if (key_flags[key])
                    return false;

                key_flags[key] = true;
                ++add_count;

                if (!verified_run) {
                    int32_t * record = (int32_t *)malloc(rec_size * sizeof(int32_t));
                    itzam_btree_find(&btree,(const void *)(&key),(void *)(record));
                    free(record);
                    ++total_finds;
                }

                break;

            case ITZAM_DUPLICATE:
                if (!key_flags[key])
                    return false;

                state = itzam_btree_remove(&btree,(const void *)(&key));
                ++total_removes;
                ++rem_count;

                if (state == ITZAM_OKAY)
                    key_flags[key] = false;
                else
                    return false;

                break;

            default:
                not_okay(state);
                return false;
        }

        if (in_transaction) {
            int32_t choice = random_int32(100);

            if (choice < chance_commit) {
                state = itzam_btree_transaction_commit(&btree);
                in_transaction = false;
                ++commit_count;
            }
            else if (choice < chance_rollback) {
                state = itzam_btree_transaction_rollback(&btree);
                in_transaction = false;

                for (i = 0; i < maxkey; ++i)
                    key_flags[i] = save_flags[i];

                add_count = save_add_count;
                rem_count = save_rem_count;

                ++roll_count;
            }
        }

        if (verified_run) {
            bool okay = verify(&btree,key_flags,maxkey, rec_size);

            if (!okay)
                return false;
        }
    }

    // verify DB if not verifying every action
    if (!verified_run) {
        bool okay = verify(&btree,key_flags,maxkey, rec_size);

        if (!okay)
            return false;
    }

    time_t elapsed = time(NULL) - start;

    int tests = add_count + rem_count;

    printf("\n       version tested: v%s\n",ITZAM_DISPLAY_VERSION);
    printf("         verification:    passed\n"); // it won't get here if verification failed
    printf("          record size: %9d bytes\n", rec_size * sizeof(int32_t));
    printf("total records   added: %9d\n", add_count);
    printf("total records removed: %9d\n", rem_count);
    printf("         total writes: %9d\n", total_writes);
    printf("    total finds/reads: %9d\n", total_finds);
    printf("        total removes: %9d\n", total_removes);

    if (use_transactions) {
        printf("         transactions: %9d\n", tran_count);
        printf("              commits: %9d\n", commit_count);
        printf("            rollbacks: %9d\n", roll_count);
    }

    printf("       database count: %9d\n", (int)btree.m_header->m_count);
    printf("      database ticker: %9d\n", (int)btree.m_header->m_ticker);
    printf("       total run time: %9u seconds\n", (unsigned int)elapsed);
    printf("operations per second: %9d\n\n", (int32_t)((double)tests / (double)elapsed));
    
    free(save_flags);

    state = itzam_btree_close(&btree);

    if (state != ITZAM_OKAY) {
        not_okay(state);
        return false;
    }

    free(key_flags);

    return true;
}

int main(int argc, char* argv[]) {
    int result = EXIT_FAILURE;

    init_test_prng((long)time(NULL));
    itzam_set_default_error_handler(error_handler);

    if (test_btree_stress())
        result = EXIT_SUCCESS;

    return result;
}

