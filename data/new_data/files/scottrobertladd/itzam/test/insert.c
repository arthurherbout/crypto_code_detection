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

//-- Report an itzam error
void not_okay(itzam_state state) {
    fprintf(stderr, "\nItzam problem: %s\n", STATE_MESSAGES[state]);
    exit(EXIT_FAILURE);
}

void error_handler(const char * function_name, itzam_error error) {
    fprintf(stderr, "Itzam error in %s: %s\n", function_name, ERROR_STRINGS[error]);
    exit(EXIT_FAILURE);
}

//-- tests
 
#define REC_SIZE 2560 /// x 4 bytes = 10240 bytes per record

typedef struct record_t {
    uint32_t m_key;
    uint32_t m_data[REC_SIZE];
}
record;

int compare_recs(const void * key1, const void * key2) {
    int result = 0;

    record * k1 = (record *)key1;
    record * k2 = (record *)key2;

    if (k1->m_key < k2->m_key)
        result = -1;
    else if (k1->m_key > k2->m_key)
        result = 1;

    return result;
}

bool test_btree_insert() {
    itzam_btree  btree;
    itzam_state  state;
    record       rec;
    int n;
    char * filename      = "insert.itz";
    itzam_int test_size  = 10000000;
    int order            = 25;

    time_t start = time(NULL);

    memset(&rec.m_data,0,REC_SIZE * sizeof(uint32_t));
    rec.m_key = 0;

    // banner for this test
    printf("\nItzam/C B-Tree Test\nStraight Insertion Performance\n");
    printf("Please wait while I insert %d records of %d bytes...\n", (int)test_size, (int)(REC_SIZE * sizeof(uint32_t)));

    state = itzam_btree_create(&btree, filename, order, sizeof(record), compare_recs, error_handler);

    if (state != ITZAM_OKAY) {
        not_okay(state);
        return false;
    }

    for (n = 1; n <= test_size; ++ n)  {
        rec.m_key = n;
        rec.m_data[0] = n;
        rec.m_data[REC_SIZE - 1] = n;

        state = itzam_btree_insert(&btree,(const void *)&rec);

        if (state != ITZAM_OKAY) {
            not_okay(state);
            return false;
        }
    }

    time_t elapsed = time(NULL) - start;

    printf("      database  count: %d\n", (int)btree.m_header->m_count);
    printf("      database ticker: %d\n", (int)btree.m_header->m_ticker);
    printf("       total run time: %d seconds\n", (int)elapsed);
    printf("insertions per second: %f\n", ((double)test_size / (double)elapsed));

    // verify database after benchmark
    itzam_btree_cursor cursor;
    state = itzam_btree_cursor_create(&cursor, &btree);

    if (state == ITZAM_OKAY) {
        do {
            // get the key pointed to by the cursor
            state = itzam_btree_cursor_read(&cursor,(void *)&rec);

            if (state == ITZAM_OKAY) {
                if ((rec.m_key != rec.m_data[0]) || (rec.m_key != rec.m_data[REC_SIZE - 1])) {
                    printf("ERROR: record retrieved for %u does not match %u or %u\n",rec.m_key,rec.m_data[0],rec.m_data[REC_SIZE - 1]);
                    break;
                }
            }
            else
                not_okay(state);
        }
        while (itzam_btree_cursor_next(&cursor));

        state = itzam_btree_cursor_free(&cursor);

        if (state != ITZAM_OKAY)
            return false;
    }

    state = itzam_btree_close(&btree);

    if (state != ITZAM_OKAY) {
        not_okay(state);
        return false;
    }

    return true;
}

int main(int argc, char* argv[]) {
    int result = EXIT_FAILURE;

    itzam_set_default_error_handler(error_handler);

    if (test_btree_insert())
        result = EXIT_SUCCESS;

    return result;
}
