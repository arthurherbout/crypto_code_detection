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

void not_okay(itzam_state state) {
    fprintf(stderr, "\nItzam problem: %s\n", STATE_MESSAGES[state]);
    exit(EXIT_FAILURE);
}

void error_handler(const char * function_name, itzam_error error) {
    fprintf(stderr, "Itzam error in %s: %s\n", function_name, ERROR_STRINGS[error]);
    exit(EXIT_FAILURE);
}

#define REC_SIZE 10
#define TEST_SIZE 10

bool test_btree_overwrite() {
    itzam_datafile  datafile;
    itzam_state     state;

    char * record;
    char * filename = "overwrite.itz";
    int n, i;
    itzam_ref w[TEST_SIZE];

    // banner for this test
    printf("\nOverwrite Testing for Itzam/C B-tree indexes\n");

    state = itzam_datafile_create(&datafile, filename);

    if (state != ITZAM_OKAY) {
        not_okay(state);
        return false;
    }

    itzam_datafile_set_error_handler(&datafile, error_handler);

    // fill datafile
    for (n = 0; n < TEST_SIZE; ++n) {
        record = (char *)malloc(REC_SIZE + 1);

        for (i = 0; i < REC_SIZE; ++i)
            record[i] = 'A' + n;

        record[REC_SIZE] = 0;

        w[n] = itzam_datafile_write(&datafile, record, REC_SIZE + 1, ITZAM_NULL_REF);

        free(record);
    }

    // display what's in the DB
    printf("\nInitial DB contents:\n");
    itzam_int len;

    for (n = 0; n < TEST_SIZE; ++n) {
        itzam_datafile_seek(&datafile, w[n]);
        itzam_datafile_read_alloc(&datafile, (void **)&record, &len);
        printf("    %s @ %d\n", record, (int)w[n]);
    }

    // modify a few records what's in the DB
    char * change = "---";
    itzam_datafile_overwrite(&datafile, (void *)change, 3, w[2], 4);
    itzam_datafile_overwrite(&datafile, (void *)change, 3, w[3], 0);
    itzam_datafile_overwrite(&datafile, (void *)change, 3, w[5], REC_SIZE - strlen(change));

    // modify a few records what's in the DB
    state = itzam_datafile_overwrite(&datafile, (void *)change, 3, w[4], 9);

    if (state != ITZAM_OVERWRITE_TOO_LONG) {
        printf("Error: Overwrite beyond end of record should have failed.\n");
        return false;
    }

    // modify a few records what's in the DB
    change = "****";
    itzam_datafile_transaction_start(&datafile);
    itzam_datafile_overwrite(&datafile, (void *)change, 3, w[8], 5);
    itzam_datafile_overwrite(&datafile, (void *)change, 3, w[9], 0);
    itzam_datafile_transaction_rollback(&datafile);

    // display what's in the DB
    printf("\nAfter overwrites, DB contents:\n");

    for (n = 0; n < TEST_SIZE; ++n) {
        itzam_datafile_seek(&datafile, w[n]);
        itzam_datafile_read_alloc(&datafile, (void **)&record, &len);
        printf("    %s @ %d\n", record, (int)w[n]);
    }

    printf("\nShould look like:\n");
    printf("    AAAAAAAAAA @ 24\n");
    printf("    BBBBBBBBBB @ 51\n");
    printf("    CCCC---CCC @ 78\n");
    printf("    ---DDDDDDD @ 105\n");
    printf("    EEEEEEEEEE @ 132\n");
    printf("    FFFFFFF--- @ 159\n");
    printf("    GGGGGGGGGG @ 186\n");
    printf("    HHHHHHHHHH @ 213\n");
    printf("    IIIIIIIIII @ 240\n");
    printf("    JJJJJJJJJJ @ 267\n\n");

    // close
    state = itzam_datafile_close(&datafile);

    if (state != ITZAM_OKAY) {
        not_okay(state);
        return false;
    }

    return true;
}

int main(int argc, char* argv[]) {
    int result = EXIT_FAILURE;

    itzam_set_default_error_handler(error_handler);

    if (test_btree_overwrite())
        result = EXIT_SUCCESS;

    return result;
}
