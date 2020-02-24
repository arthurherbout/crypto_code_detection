#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <assert.h>

/* for mkdir */
#include <sys/stat.h>

#include "lmdb.h"
#include "lmdb_helpers.h"

void mdb_db_create(MDB_dbi *dbi, MDB_env *env, const char* db_name)
{
    int e;
    MDB_txn *txn;

    e = mdb_txn_begin(env, NULL, 0, &txn);
    if (0 != e)
        mdb_fatal(e);

    e = mdb_dbi_open(txn, db_name, MDB_CREATE, dbi);
    if (0 != e)
        mdb_fatal(e);

    e = mdb_txn_commit(txn);
    if (0 != e)
        mdb_fatal(e);
}

void mdb_db_env_create(
        MDB_env **env,
	unsigned int flags,
        const char* path,
        int size_mb)
{
    int e;

    mkdir(path, 0777);

    e = mdb_env_create(env);
    if (0 != e)
        mdb_fatal(e);

    e = mdb_env_set_mapsize(*env, size_mb * 1024 * 1024);
    if (0 != e)
        mdb_fatal(e);

    e = mdb_env_set_maxdbs(*env, 1024);
    if (0 != e)
        mdb_fatal(e);

    e = mdb_env_open(*env, path, flags, 0664);
    if (22 == e)
    {
        fprintf(stderr,
                "ERROR:\tThe current database file path (%s) is not mmap-able\n"
                "\tPlease consider using a different database path\n", path);
        exit(1);
    }
    else if (0 != e)
        mdb_fatal(e);
}

void mdb_print_db_stats(MDB_dbi dbi, MDB_env *env)
{
    int e;
    MDB_stat stat;
    MDB_txn *txn;

    e = mdb_txn_begin(env, NULL, 0, &txn);
    if (0 != e)
        mdb_fatal(e);

    e = mdb_stat(txn, dbi, &stat);
    if (0 != e)
        mdb_fatal(e);

    printf("ms_psize: %d\n", stat.ms_psize);
    printf("ms_depth: %d\n", stat.ms_depth);
    printf("ms_branch_pages: %ld\n", stat.ms_branch_pages);
    printf("ms_leaf_pages: %ld\n", stat.ms_leaf_pages);
    printf("ms_overflow_pages: %ld\n", stat.ms_overflow_pages);
    printf("ms_entries: %ld\n", stat.ms_entries);

    e = mdb_txn_commit(txn);
    if (0 != e)
        mdb_fatal(e);
}
