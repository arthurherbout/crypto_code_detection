#ifndef SCHEMA_CACHE_H
#define SCHEMA_CACHE_H

#include "oid2avro.h"
#include "utils/hsearch.h"

typedef struct {
    Oid                 relid;       /* Oid of the table. Used as key in hash table, so it must be first in struct */
    NameData            relname;     /* Name of the table */
    Oid                 ns_id;       /* Oid of the namespace containing this table */
    NameData            ns_name;     /* Name of the namespace containing this table */
    Oid                 key_id;      /* Oid of the primary key or replica identity index */
    NameData            key_name;    /* Name of the primary key or replica identity index */
    Oid                 keyns_id;    /* Oid of the namespace of the primary key index */
    NameData            keyns_name;  /* Name of the namespace of the primary key index */
    TupleDesc           key_tupdesc; /* Postgres tuple descriptor for primary key or replica identity index */
    TupleDesc           row_tupdesc; /* Postgres tuple descriptor for a row of this table */
    avro_schema_t       key_schema;  /* Avro schema for the table's primary key or replica identity */
    avro_schema_t       row_schema;  /* Avro schema for one row of the table */
    avro_value_iface_t *key_iface;   /* Avro generic interface for creating key values */
    avro_value_iface_t *row_iface;   /* Avro generic interface for creating row values */
    avro_value_t        key_value;   /* Avro key value, for encoding one key */
    avro_value_t        row_value;   /* Avro row value, for encoding one row */
} schema_cache_entry;

typedef struct {
    MemoryContext context;         /* Context in which cache entries are allocated */
    HTAB *entries;                 /* Hash table mapping Oid to schema_cache_entry */
} schema_cache;

typedef schema_cache *schema_cache_t;

schema_cache_t schema_cache_new(MemoryContext context);
int schema_cache_lookup(schema_cache_t cache, Relation rel, schema_cache_entry **entry_out);
void schema_cache_free(schema_cache_t cache);
char *schema_debug_info(Relation rel, TupleDesc tupdesc);

#endif /* SCHEMA_CACHE_H */
