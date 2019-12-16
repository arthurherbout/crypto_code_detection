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

//-- error handling

void not_okay(itzam_state state)
{
    fprintf(stderr, "\nItzam problem: %s\n", STATE_MESSAGES[state]);
    exit(EXIT_FAILURE);
}

void error_handler(const char * function_name, itzam_error error)
{
    fprintf(stderr, "Itzam error in %s: %s\n", function_name, ERROR_STRINGS[error]);
    exit(EXIT_FAILURE);
}

//-- tests

bool test_btree_befaft() {
    bool result = false, bsuccess = false, asuccess = false;
    int n;
    const uint16_t order = 5; // small, so we get lots of tree changes
    time_t start;
    int32_t rec, before, after;
    
    char * filename = "relative.itz";
    
    itzam_btree btree;
    itzam_state state;
    
    // banner for this test
    printf("\nTesting Itzam/C b-tree indexes with new \"nearby\" search functions\n");
    
    // create an empty database file
    state = itzam_btree_create(&btree, filename, order, sizeof(int32_t), itzam_comparator_int32, error_handler);
    
    if (state != ITZAM_OKAY) {
        printf("uable to create index file\n");
        return false;
    }
    
    // fill the btree with a known sequence
    int32_t largest = -1;
    int32_t x = -1;
    
    for (int n = 0; n < 100; ++n) {
        x += random_int32(3) + 1;
        largest = x;
        
        state = itzam_btree_insert(&btree, &x);
        
        if (state != ITZAM_OKAY) {
            printf("could not add %d to index file\n",n);
            return false;
        }
    }
    
    for (int n = -1; n < largest + 1; ++n) {
        before = -1;
        after = -1;
        
        // look before
        bsuccess = itzam_btree_find_before(&btree, &n, &before);
        
        // look after
        asuccess = itzam_btree_find_after(&btree, &n, &after);
        
        // print before (target) after
        printf("%8d (%s) <= %8d <= %8d (%s)\n", before, bsuccess ? "Y" : "N", n, after, asuccess ? "Y" : "N"); 
    }
    
    // done
    return true;
}

int main(int argc, char* argv[]) {
    int result = EXIT_FAILURE;

    init_test_prng((int32_t)time(NULL));
    itzam_set_default_error_handler(error_handler);

    if (test_btree_befaft())
        result = EXIT_SUCCESS;

    return result;
}

