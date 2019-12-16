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
#include <sched.h>

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
void not_okay(itzam_state state){
    fprintf(stderr, "\nItzam problem: %s\n", STATE_MESSAGES[state]);
    exit(EXIT_FAILURE);
}

void error_handler(const char * function_name, itzam_error error) {
    fprintf(stderr, "Itzam error in %s: %s\n", function_name, ERROR_STRINGS[error]);
    exit(EXIT_FAILURE);
}

//-- test routines

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

bool test_btree_thread(itzam_btree * btree, int maxkey, int test_size, int * completed)
{
    itzam_state state = ITZAM_FAILED;
    int32_t key;
    int i;

    if (btree != NULL) {
        for (i = 0; i < test_size; ++i) {
            key = (int32_t)random_int32((int32_t)maxkey);

            if (ITZAM_OKAY != itzam_btree_insert(btree,(const void *)&key))  {
                // ignore error here, as the record may have been removed by another thread
                itzam_btree_remove(btree,(const void *)&key);
            }
        }

        state = ITZAM_OKAY;

        pthread_mutex_lock(&mutex);

        if (completed != NULL)
            (*completed)++;

        pthread_mutex_unlock(&mutex);
    }

    if (state == ITZAM_OKAY)
        return true;
    else
        return false;
}

struct threadArgs {
    itzam_btree * btree;
    int maxkey;
    int test_size;
    int * completed;
};

void * threadProc(void * a) {
    struct threadArgs * args = (struct threadArgs *)a;
    test_btree_thread(args->btree, args->maxkey, args->test_size, args->completed);
    return NULL;
}

static void createdb(const char * filename, uint16_t order) {
    itzam_btree btree;
    itzam_state state;

    state = itzam_btree_create(&btree, filename, order, sizeof(int32_t), itzam_comparator_int32, error_handler);

    if (state != ITZAM_OKAY) {
        printf("Unable to create B-tree index file %s\n", filename);
        exit(1);
    }

    itzam_btree_close(&btree);
}

static bool test_threaded() {
    const uint16_t order = 11;
    char * sfilename = "sthreaded.itz";
    char * mfilename = "mthreaded.itz";
    time_t start;
    int maxkey = 10000;
    int test_size = 10000000;
    pthread_attr_t attr;
    pthread_t * thread;
    struct threadArgs * args;
    int completed = 0;
    int32_t key;
    int n;

    itzam_btree btree;
    itzam_state state;

    int num_threads = (int)sysconf(_SC_NPROCESSORS_ONLN);

    if (num_threads < 2)
        num_threads = 2;

    thread = (pthread_t *)malloc(num_threads * sizeof(pthread_t));
    args = (struct threadArgs *)malloc(num_threads * sizeof(struct threadArgs));

    printf("\nItzam/C B-Tree Test\nMultiple threads, simultaneously using the same Itzam index\n\n");
    printf("Parameters:\n%10d B-tree order\n%10d unique keys\n%10d threads\n%10d insert & remove operations\n", order, maxkey, num_threads, test_size);

    createdb(mfilename, order);

    test_size /= num_threads;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_attr_setschedpolicy(&attr, SCHED_RR);

    state = itzam_btree_open(&btree, mfilename, itzam_comparator_int32, error_handler, false, false);

    if (state != ITZAM_OKAY) {
        printf("Unable to open B-tree index file %s\n",mfilename);
        return false;
    }

    printf("\nPerforming multi-thread test... ");
    fflush(stdout);

    start = time(NULL);

    for (n = 0; n < num_threads; ++n) {
        args[n].btree = &btree;
        args[n].maxkey = maxkey;
        args[n].test_size = test_size;
        args[n].completed = &completed;
        pthread_create(&thread[n], &attr, threadProc, &args[n]);
    }

    while (completed < num_threads)
        usleep(100000);

    itzam_btree_close(&btree);

    free(thread);
    free(args);

    time_t melapsed = time(NULL) - start;

    printf("done\n\n%10d seconds run time\n\n", (int)melapsed);

    return true;
}

int main(int argc, char* argv[]) {
    int result = EXIT_FAILURE;

    init_test_prng((long)time(NULL));
    itzam_set_default_error_handler(error_handler);

    if (test_threaded())
        result = EXIT_SUCCESS;

    return result;
}

