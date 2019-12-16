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

/*----------------------------------------------------------
 * embedded random number generator; ala Park and Miller
 */
static int32_t seed = 1325;

void init_test_prng(int32_t s)
{
    seed = s;
}

int32_t random_int32(int32_t limit)
{
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

/*----------------------------------------------------------
 *  Reports an itzam error
 */
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

/*----------------------------------------------------------
 * Key record
 */
#define MAX_KEY_LEN  24
#define REC_LEN_BASE 10
#define REC_LEN_EXT  90

typedef struct t_key_type
{
    char      m_key[MAX_KEY_LEN];
    itzam_int m_rec_len;
    itzam_ref m_rec_ref;
}
key_type;

// could be shorter, but want to be clear
int compare_keys(const void * key1, const void * key2)
{
    const key_type * k1 = (const key_type *)key1;
    const key_type * k2 = (const key_type *)key2;

    return strcmp(k1->m_key,k2->m_key);
}

/*----------------------------------------------------------
 * tests
 */
bool test_btree_strvar()
{
    itzam_btree  btree;
    itzam_btree_cursor cursor;
    itzam_state  state;
    int          n, i, klen;
    char *       filename  = "strvar.itz";
    itzam_int    test_size = 100;
    int          order = 25;
    key_type     key;
    char *       record;
    itzam_int    len_read;

    // banner for this test
    printf("\nItzam/C B-Tree Test\nString Keys & Variable-Length Records\n");

    state = itzam_btree_create(&btree, filename, order, sizeof(key_type), compare_keys, error_handler);

    if (state != ITZAM_OKAY)
        not_okay(state);

    for (n = 1; n <= test_size; ++ n)
    {
        /* generate a random alphabetic key */
        memset(key.m_key,0,MAX_KEY_LEN);

        klen = random_int32((int32_t)(MAX_KEY_LEN - 9)) + 9;

        for (i = 0; i < klen; ++i)
            key.m_key[i] = 'A' + random_int32(26);

        /* generate the record */
        key.m_rec_len = REC_LEN_BASE + (itzam_int)random_int32((int32_t)REC_LEN_EXT);

        record = (char *)malloc(key.m_rec_len);

        memset(record, key.m_key[0], key.m_rec_len);

        /* write record directly to the datafile */
        key.m_rec_ref = itzam_datafile_write(btree.m_datafile, (void *)record, key.m_rec_len, ITZAM_NULL_REF);

        /* if it worked, write the key */
        if (key.m_rec_ref != ITZAM_NULL_REF)
        {
            state = itzam_btree_insert(&btree,(const void *)&key);

            if (ITZAM_OKAY != state)
                not_okay(state);
        }
        else
        {
            fprintf(stderr,"Unable to write database record\n");
            exit(EXIT_FAILURE);
        }

        printf("wrote record at key %s\n",key.m_key);

        free(record);
    }

    // now use a cursor to show the records stored, in key order
    state = itzam_btree_cursor_create(&cursor, &btree);

    if (state == ITZAM_OKAY)
    {
        do
        {
            // get the key pointed to by the cursor
            state = itzam_btree_cursor_read(&cursor,(void *)&key);

            if (state == ITZAM_OKAY)
            {
                // seek to record
                state = itzam_datafile_seek(btree.m_datafile, key.m_rec_ref);

                if (state == ITZAM_OKAY)
                {
                    // read record
                    state = itzam_datafile_read_alloc(btree.m_datafile,(void **)&record,&len_read);

                    if (state == ITZAM_OKAY)
                    {
                        // verify that record matches key
                        for (i = 0; i < key.m_rec_len; ++i)
                        {
                            if (key.m_key[0] != record[i])
                            {
                                printf("ERROR: record retrieved for %s does not match\n",key.m_key);
                                break;
                            }
                        }

                        printf("record retrieved for %s matches\n",key.m_key);

                        free(record); // was allocated by the datafile read;
                    }
                    else
                        not_okay(state);
                }
                else
                    not_okay(state);
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

    if (state != ITZAM_OKAY)
        return false;

    return true;
}

int main(int argc, char* argv[])
{
    int result = EXIT_FAILURE;

    init_test_prng((long)time(NULL));
    itzam_set_default_error_handler(error_handler);

    if (test_btree_strvar())
        result = EXIT_SUCCESS;

    return result;
}

