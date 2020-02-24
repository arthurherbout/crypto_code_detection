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

#include "itzam.h"
#include <stdlib.h>
#include <ctype.h>

#if defined(ITZAM_LINUX)
static pthread_mutex_t global_mutex = PTHREAD_MUTEX_INITIALIZER;
#endif

static itzam_state update_header(itzam_btree * btree) {
    itzam_state result = ITZAM_FAILED;

    // rewrite file header
    itzam_ref where = itzam_datafile_write_flags(btree->m_datafile, btree->m_header, sizeof(itzam_btree_header), btree->m_header->m_where, ITZAM_RECORD_BTREE_HEADER);

    if (where == btree->m_header->m_where)
        result = ITZAM_OKAY;

    return result;
}

static itzam_btree_page * dupe_page(const itzam_btree * btree, const itzam_btree_page * source_page) {
    itzam_btree_page * page = NULL;

    // allocate page
    page = (itzam_btree_page *)malloc(sizeof(itzam_btree_page));

    if (page != NULL) {
        // allocate data space
        page->m_data = (itzam_byte *)malloc(btree->m_header->m_sizeof_page);

        if (page->m_data != NULL)  {
            page->m_header = (itzam_btree_page_header *)page->m_data;
            page->m_keys   = (itzam_byte *)(page->m_data + sizeof(itzam_btree_page_header));
            page->m_links  = (itzam_ref *)(page->m_data + sizeof(itzam_btree_page_header) + btree->m_header->m_sizeof_key * btree->m_header->m_order);
            memcpy(page->m_data, source_page->m_data, btree->m_header->m_sizeof_page);
        }
        else {
            free(page);
            page = NULL;
        }
    }

    return page;
}

static void set_page_pointers(const itzam_btree * btree, itzam_btree_page * page) {
    page->m_header = (itzam_btree_page_header *)page->m_data;
    page->m_keys   = (itzam_byte *)(page->m_data + sizeof(itzam_btree_page_header));
    page->m_links  = (itzam_ref *)(page->m_data + sizeof(itzam_btree_page_header) + btree->m_header->m_sizeof_key * btree->m_header->m_order);
}

static void init_page(const itzam_btree * btree, itzam_btree_page * page) {
    int n;

    page->m_header->m_where     = ITZAM_NULL_REF;
    page->m_header->m_parent    = ITZAM_NULL_REF;
    page->m_header->m_key_count = 0;

    memset(page->m_keys, 0, btree->m_header->m_sizeof_key * btree->m_header->m_order);

    for (n = 0; n < btree->m_header->m_order + 1; ++n)
        page->m_links[n] = ITZAM_NULL_REF;
}

static void set_page(const itzam_btree * btree, itzam_btree_page * page, itzam_byte * memory) {
    if ((page != NULL) && (memory != NULL)) {
        page->m_data = memory;
        set_page_pointers(btree,page);
    }
}

static itzam_btree_page * alloc_page(const itzam_btree * btree) {
    itzam_btree_page * page = NULL;

    // allocate page
    page = (itzam_btree_page *)malloc(sizeof(itzam_btree_page));

    if (page != NULL) {
        // allocate data space
        page->m_data = (itzam_byte *)malloc(btree->m_header->m_sizeof_page);

        if (page->m_data != NULL) {
            set_page_pointers(btree,page);
            init_page(btree,page);
        }
        else {
            // error; clean up and return NULL
            free(page);
            page = NULL;
        }
    }

    return page;
}

static void free_page(itzam_btree_page * page) {
    if ((page != NULL) && (page->m_header->m_parent != ITZAM_NULL_REF)) {
        if (page->m_data != NULL)
            free(page->m_data);

        free(page);
    }
}

static void set_root(itzam_btree * btree, itzam_btree_page * new_root) {
    memcpy(btree->m_root_data, new_root->m_data, btree->m_header->m_sizeof_page);
    btree->m_header->m_root_where = new_root->m_header->m_where;
    update_header(btree);
    new_root->m_data = NULL;
}

// can't be static because it's useful for debug/analysis routines
itzam_btree_page * read_page(itzam_btree * btree, itzam_ref where) {
    itzam_btree_page * page = NULL;

    // position datafile
    if (ITZAM_OKAY == itzam_datafile_seek(btree->m_datafile,where)) {
        // allocate a buffer
        page = alloc_page(btree);

        if (page != NULL)  {
            // read the ref
            if (ITZAM_OKAY != itzam_datafile_read(btree->m_datafile,page->m_data,btree->m_header->m_sizeof_page)) {
                free_page(page);
                page = NULL;
            }
        }
    }

    return page;
}

static itzam_ref write_page(itzam_btree * btree, itzam_btree_page * page) {
    itzam_ref where;

    // does this page have a location, i.e., is it new?
    if (page->m_header->m_where == ITZAM_NULL_REF)
        page->m_header->m_where = itzam_datafile_get_next_open(btree->m_datafile, btree->m_header->m_sizeof_page);

    where = itzam_datafile_write_flags(btree->m_datafile, page->m_data, btree->m_header->m_sizeof_page, page->m_header->m_where, ITZAM_RECORD_BTREE_PAGE);

    // make sure things got put where we thought they did
    if (where != page->m_header->m_where)
        where = ITZAM_NULL_REF;

    return where;
}

int itzam_comparator_int32(const void * key1, const void * key2) {
    int result = 0;

    int32_t * k1 = (int32_t *)key1;
    int32_t * k2 = (int32_t *)key2;

    if (*k1 < *k2)
        result = -1;
    else if (*k1 > *k2)
        result = 1;

    return result;
}

int itzam_comparator_uint32(const void * key1, const void * key2) {
    int result = 0;

    uint32_t * k1 = (uint32_t *)key1;
    uint32_t * k2 = (uint32_t *)key2;

    if (*k1 < *k2)
        result = -1;
    else if (*k1 > *k2)
        result = 1;

    return result;
}

int itzam_comparator_int64(const void * key1, const void * key2) {
    int result = 0;

    int64_t * k1 = (int64_t *)key1;
    int64_t * k2 = (int64_t *)key2;

    if (*k1 < *k2)
        result = -1;
    else if (*k1 > *k2)
        result = 1;

    return result;
}

int itzam_comparator_uint64(const void * key1, const void * key2) {
    int result = 0;

    uint64_t * k1 = (uint64_t *)key1;
    uint64_t * k2 = (uint64_t *)key2;

    if (*k1 < *k2)
        result = -1;
    else if (*k1 > *k2)
        result = 1;

    return result;
}

int itzam_comparator_string(const void * key1, const void * key2) {
    return strcmp((const char *)key1,(const char *)key2);
}

static char * get_shared_name(const char * fmt, const char * filename) {
    char * result = (char *)malloc(strlen(fmt) + strlen(filename) + 1);

    char * norm = strdup(filename);
    char * c = norm;

    while (*c) {
        if (!isalnum(*c))
            *c = '_';
        else
            if (isalpha(*c))
                *c = tolower(*c);

        ++c;
    }

    sprintf(result, fmt, norm);

    free(norm);

    return result;
}

#if defined(ITZAM_LINUX)
static const char * HDR_NAME_MASK = "/%s-ItzamBTreeHeader";
static const char * ROOT_NAME_MASK = "/%s-ItzamBTreeRoot";
#else
static const char * HDR_NAME_MASK = "Global\\%s-ItzamBTreeHeader";
static const char * ROOT_NAME_MASK = "Global\\%s-ItzamBTreeRoot";
#endif

#define MAKE_ITZAM_BHNAME(basename) get_shared_name(HDR_NAME_MASK,basename)
#define MAKE_ITZAM_ROOT_NAME(basename) get_shared_name(ROOT_NAME_MASK,basename)

itzam_state itzam_btree_create(itzam_btree * btree, const char * filename, uint16_t order, itzam_int key_size, itzam_key_comparator * key_comparator, itzam_error_handler * error_handler) {
    itzam_state result = ITZAM_FAILED;
    bool creator;

    #if defined(ITZAM_LINUX)
        pthread_mutex_lock(&global_mutex);
    #endif

    // make sure the arguments make sense
    if ((btree != NULL) && (filename != NULL) && (key_size > 0) && (key_comparator != NULL)) {
        // allocate datafile
        btree->m_datafile = (itzam_datafile *)malloc(sizeof(itzam_datafile));

        if (btree->m_datafile != NULL)  {
            // create data file
            result = itzam_datafile_create(btree->m_datafile,filename);

            if (ITZAM_OKAY == result) {
                // If an error handler was provided, assign it to the datafile
                if (error_handler != NULL)
                    itzam_datafile_set_error_handler(btree->m_datafile,error_handler);

                // allocate memory for shared header
                btree->m_shmem_header_name = MAKE_ITZAM_BHNAME(filename);
                btree->m_shmem_header = itzam_shmem_obtain(btree->m_shmem_header_name, sizeof(itzam_btree_header),&creator);
                btree->m_header = (itzam_btree_header *)itzam_shmem_getptr(btree->m_shmem_header, sizeof(itzam_btree_header));

                // fill in structure
                if (order < ITZAM_BTREE_ORDER_MINIMUM)
                    order = ITZAM_BTREE_ORDER_MINIMUM;

                btree->m_header->m_version     = ITZAM_BTREE_VERSION;
                btree->m_header->m_order       = order;
                btree->m_header->m_count       = 0;
                btree->m_header->m_ticker      = 0;
                btree->m_header->m_schema_ref  = ITZAM_NULL_REF;

                btree->m_free_datafile         = true;
                btree->m_links_size            = btree->m_header->m_order + 1;
                btree->m_min_keys              = btree->m_header->m_order / 2;
                btree->m_key_comparator        = key_comparator;
                btree->m_cursor_count          = 0;

                btree->m_header->m_where       = itzam_datafile_get_next_open(btree->m_datafile,sizeof(itzam_btree_header));
                btree->m_header->m_root_where  = 0;
                btree->m_header->m_sizeof_key  = key_size;
                btree->m_header->m_sizeof_page = sizeof(itzam_btree_page_header) + btree->m_header->m_sizeof_key * btree->m_header->m_order + sizeof(itzam_ref) * btree->m_links_size;

                // write header for first time (lacks root pointer info, but needs to occupy space in the file)
                if (btree->m_header->m_where == itzam_datafile_write_flags(btree->m_datafile, btree->m_header, sizeof(itzam_btree_header), btree->m_header->m_where, ITZAM_RECORD_BTREE_HEADER))  {
                    // allocate memory for shared header
                    btree->m_shmem_root_name = MAKE_ITZAM_ROOT_NAME(filename);
                    btree->m_shmem_root = itzam_shmem_obtain(btree->m_shmem_root_name, btree->m_header->m_sizeof_page, &creator);
                    btree->m_root_data  = (itzam_byte *)itzam_shmem_getptr(btree->m_shmem_root, btree->m_header->m_sizeof_page);
                    set_page(btree, &btree->m_root, btree->m_root_data);
                    init_page(btree, &btree->m_root);

                    // assign root a file position
                    btree->m_root.m_header->m_where = itzam_datafile_get_next_open(btree->m_datafile, btree->m_header->m_sizeof_page);

                    if (btree->m_root.m_header->m_where != ITZAM_NULL_REF) {
                        // write root page
                        btree->m_header->m_root_where = itzam_datafile_write_flags(btree->m_datafile, btree->m_root.m_data, btree->m_header->m_sizeof_page, btree->m_root.m_header->m_where,  ITZAM_RECORD_BTREE_PAGE);

                        // make certain that the root was written where we expect it to be
                        if ((btree->m_header->m_root_where == btree->m_root.m_header->m_where) &&  (btree->m_header->m_root_where != ITZAM_NULL_REF))
                            result = update_header(btree);

                        // make certain root was written
                        itzam_file_commit(btree->m_datafile->m_file);
                    }
                }

                // remember to deallocate this datafile at closing
                btree->m_free_datafile = true;

                result = ITZAM_OKAY;
            }
        }
    }
    else
        default_error_handler("itzam_btree_create",ITZAM_ERROR_INVALID_DATAFILE_OBJECT);

    #if defined(ITZAM_LINUX)
        pthread_mutex_unlock(&global_mutex);
    #endif

    return result;
}

itzam_state itzam_btree_open(itzam_btree * btree, const char * filename,  itzam_key_comparator * key_comparator, itzam_error_handler * error_handler,  bool recover, bool read_only) {
    // what we return
    itzam_state result = ITZAM_FAILED;
    bool creator;

    #if defined(ITZAM_LINUX)
        pthread_mutex_lock(&global_mutex);
    #endif

    // make sure the arguments make sense
    if ((btree != NULL) && (filename != NULL) && (key_comparator != NULL)) {
        // allocate datafile
        btree->m_datafile = (itzam_datafile *)malloc(sizeof(itzam_datafile));

        if (btree->m_datafile != NULL) {
            // create data file
            result = itzam_datafile_open(btree->m_datafile,filename,recover,read_only);

            if (ITZAM_OKAY == result) {
                // If an error handler was provided, assign it to the datafile
                if (error_handler != NULL)
                    itzam_datafile_set_error_handler(btree->m_datafile,error_handler);

                // do the actual create
                btree->m_free_datafile = true;
                btree->m_key_comparator = key_comparator;
                btree->m_cursor_count = 0;

                // allocate memory for embedded header
                btree->m_shmem_header_name = MAKE_ITZAM_BHNAME(filename);
                btree->m_shmem_header = itzam_shmem_obtain(btree->m_shmem_header_name, sizeof(itzam_btree_header),&creator);
                btree->m_header = (itzam_btree_header *)itzam_shmem_getptr(btree->m_shmem_header, sizeof(itzam_btree_header));

                // assumes first record is header
                if (ITZAM_OKAY == itzam_datafile_rewind(btree->m_datafile)) {
                    if (creator)
                        result = itzam_datafile_read(btree->m_datafile,btree->m_header,sizeof(itzam_btree_header));

                    if (ITZAM_OKAY == result) {
                        // verify version
                        if (btree->m_header->m_version == ITZAM_BTREE_VERSION)  {
                            // finish intializing btree
                            btree->m_links_size = btree->m_header->m_order + 1;
                            btree->m_min_keys   = btree->m_header->m_order / 2;

                            // allocate memory for shared header
                            btree->m_shmem_root_name = MAKE_ITZAM_ROOT_NAME(filename);
                            btree->m_shmem_root = itzam_shmem_obtain(btree->m_shmem_root_name, btree->m_header->m_sizeof_page,&creator);
                            btree->m_root_data  = (itzam_byte *)itzam_shmem_getptr(btree->m_shmem_root, btree->m_header->m_sizeof_page);
                            set_page(btree, &btree->m_root, btree->m_root_data);

                            // read root
                            if (creator) {
                                itzam_datafile_seek(btree->m_datafile, btree->m_header->m_root_where);
                                itzam_datafile_read(btree->m_datafile, btree->m_root_data, btree->m_header->m_sizeof_page);
                            }

                            result = ITZAM_OKAY;
                        }
                        else
                            result = ITZAM_VERSION_ERROR;
                    }
                }

                result = ITZAM_OKAY;
            }
        }
    }
    else
        default_error_handler("itzam_btree_open",ITZAM_ERROR_INVALID_DATAFILE_OBJECT);

    #if defined(ITZAM_LINUX)
        pthread_mutex_unlock(&global_mutex);
    #endif

    return result;
}

// return number of active records in database
uint64_t itzam_btree_count(itzam_btree * btree) {
    uint64_t result = 0;

    if (btree != NULL) {
        itzam_datafile_mutex_lock(btree->m_datafile);

        if (ITZAM_OKAY == result)
            result = btree->m_header->m_count;

        itzam_datafile_mutex_unlock(btree->m_datafile);
    }

    return result;
}

// return ticker, which is a count of every record ever added to the database
uint64_t itzam_btree_ticker(itzam_btree * btree) {
    uint64_t result = 0;

    if (btree != NULL) {
        itzam_datafile_mutex_lock(btree->m_datafile);

        if (ITZAM_OKAY == result)
            result = btree->m_header->m_ticker;

        itzam_datafile_mutex_unlock(btree->m_datafile);
    }
    else
        default_error_handler("itzam_btree_ticker",ITZAM_ERROR_INVALID_DATAFILE_OBJECT);

    return result;
}

// close file
itzam_state itzam_btree_close(itzam_btree * btree) {
    itzam_state result = ITZAM_FAILED;

    #if defined(ITZAM_LINUX)
        pthread_mutex_lock(&global_mutex);
    #endif

    // make sure the arguments make sense
    if ((btree != NULL) && (btree->m_cursor_count == 0)) {
        if (!btree->m_datafile->m_read_only) {
            itzam_datafile_mutex_unlock(btree->m_datafile);
            update_header(btree);
            itzam_datafile_mutex_unlock(btree->m_datafile);
        }

        if (result == ITZAM_OKAY) {
            free(btree->m_datafile);
            btree->m_datafile = NULL;
        }

        itzam_shmem_freeptr(btree->m_root_data, btree->m_header->m_sizeof_page);
        itzam_shmem_close(btree->m_shmem_root, btree->m_shmem_root_name);
        free(btree->m_shmem_root_name);

        itzam_shmem_freeptr(btree->m_header, sizeof(itzam_btree_header));
        itzam_shmem_close(btree->m_shmem_header,btree->m_shmem_header_name);
        free(btree->m_shmem_header_name);

        result = ITZAM_OKAY;
    }
    else
        default_error_handler("itzam_btree_close",ITZAM_ERROR_INVALID_DATAFILE_OBJECT);

    #if defined(ITZAM_LINUX)
        pthread_mutex_unlock(&global_mutex);
    #endif

    return result;
}

void itzam_btree_mutex_lock(itzam_btree * btree) {
    if (btree != NULL)
        itzam_datafile_mutex_lock(btree->m_datafile);
    else
        default_error_handler("itzam_btree_lock",ITZAM_ERROR_INVALID_DATAFILE_OBJECT);
}

void itzam_btree_mutex_unlock(itzam_btree * btree) {
    if (btree != NULL)
        itzam_datafile_mutex_unlock(btree->m_datafile);
    else
        default_error_handler("itzam_btree_unlock",ITZAM_ERROR_INVALID_DATAFILE_OBJECT);
}

// lock btree file
bool itzam_btree_file_lock(itzam_btree * btree) {
    bool result = false;

    if (btree != NULL) {
        itzam_datafile_mutex_lock(btree->m_datafile);
        result = itzam_datafile_file_lock(btree->m_datafile);
        itzam_datafile_mutex_unlock(btree->m_datafile);
    }
    else
        default_error_handler("itzam_btree_lock",ITZAM_ERROR_INVALID_DATAFILE_OBJECT);

    return result;
}

// unlock btree file
bool itzam_btree_file_unlock(itzam_btree * btree) {
    bool result = false;

    if (btree != NULL) {
        itzam_datafile_mutex_lock(btree->m_datafile);
        result = itzam_datafile_file_unlock(btree->m_datafile);
        itzam_datafile_mutex_unlock(btree->m_datafile);
    }
    else
        default_error_handler("itzam_btree_unlock",ITZAM_ERROR_INVALID_DATAFILE_OBJECT);

    return result;
}

bool itzam_btree_is_open(itzam_btree * btree) {
    bool result = false;

    if (btree != NULL) {
        itzam_datafile_mutex_lock(btree->m_datafile);
        result = itzam_datafile_is_open(btree->m_datafile);
        itzam_datafile_mutex_unlock(btree->m_datafile);
    }
    else
        default_error_handler("itzam_btree_is_open",ITZAM_ERROR_INVALID_DATAFILE_OBJECT);

    return result;
}

void itzam_btree_set_error_handler(itzam_btree * btree, itzam_error_handler * error_handler) {
    if (btree != NULL) {
        itzam_datafile_mutex_lock(btree->m_datafile);
        itzam_datafile_set_error_handler(btree->m_datafile,error_handler);
        itzam_datafile_mutex_unlock(btree->m_datafile);
    }
    else
        default_error_handler("itzam_btree_set_error_handler",ITZAM_ERROR_INVALID_DATAFILE_OBJECT);
}

// structure used to return search information
typedef struct {
    itzam_btree_page * m_page;
    int                m_index;
    bool               m_found;
} search_result;

static void search(itzam_btree * btree, const void * key, search_result * result) {
    int index;

    // start at root
    itzam_btree_page * page = &btree->m_root;

    while (true) {
       index = 0;

        // if page is empty, we didn't find the key
        if ((page == NULL) || (page->m_header->m_key_count == 0)) {
            result->m_page  = page;
            result->m_index = index;
            result->m_found = false;
            return;
        }
        else {
            // loop through keys
            while (index < page->m_header->m_key_count) {
                int comp = btree->m_key_comparator(key,(const void *)(page->m_keys + index * btree->m_header->m_sizeof_key));

                // do we move on, or have we found it?
                 
                if (comp > 0)
                    ++index;
                else {
                    if (comp == 0) {
                        result->m_page  = page;
                        result->m_index = index;
                        result->m_found = true;
                        return;
                    }

                    break;
                }
            }

            // if we're in a leaf, the key hasn't been found
            if (page->m_links[index] == ITZAM_NULL_REF) {
                result->m_page  = page;
                result->m_index = index;
                result->m_found = false;
                return;
            }
            else  {
                // read and search next page
                itzam_btree_page * next_page = read_page(btree,page->m_links[index]);

                if (page->m_header->m_parent != ITZAM_NULL_REF)
                    free_page(page);

                page = next_page;
            }
        }
    }
}

bool itzam_btree_find(itzam_btree * btree, const void * key, void * returned_key) {
    search_result s;
    s.m_found = false;
    s.m_index = 0;
    s.m_page  = NULL;

    if ((btree != NULL) && (key != NULL)) {
        itzam_datafile_mutex_lock(btree->m_datafile);

        search(btree,key,&s);

        if ((s.m_found) && (returned_key != NULL))
            memcpy(returned_key,s.m_page->m_keys + s.m_index * btree->m_header->m_sizeof_key, btree->m_header->m_sizeof_key);

        if (s.m_page->m_header->m_parent != ITZAM_NULL_REF)
            free_page(s.m_page);

        itzam_datafile_mutex_unlock(btree->m_datafile);
    }
    else
        default_error_handler("itzam_btree_find",ITZAM_ERROR_INVALID_DATAFILE_OBJECT);

    return s.m_found;
}

static bool search_before(itzam_btree * btree, const void * key, void * result) {
    int index;
    bool success = false;
    
    // start at root
    itzam_btree_page * page = &btree->m_root;
    
    while (true) {
        index = 0;
        
        // if page is empty, we didn't find the key
        if ((page == NULL) || (page->m_header->m_key_count == 0))
            return false;
        else {
            // loop through keys
            while (index < page->m_header->m_key_count) {
                int comp = btree->m_key_comparator(key,(const void *)(page->m_keys + index * btree->m_header->m_sizeof_key));
                
                if (comp < 0) // key < page key
                    break;
                else if (comp == 0) {
                    memcpy(result,(const void *)(page->m_keys + index * btree->m_header->m_sizeof_key),btree->m_header->m_sizeof_key);
                    return true;
                }
                else if (comp > 0) {
                    // key > page_key
                    memcpy(result,(const void *)(page->m_keys + index * btree->m_header->m_sizeof_key),btree->m_header->m_sizeof_key);
                    success = true;
                    ++index;
                }
            }
            
            // if we're in a leaf, we're done
            if (page->m_links[index] == ITZAM_NULL_REF)
                return success;
            else {
                // read and search next page
                itzam_btree_page * next_page = read_page(btree,page->m_links[index]);
                
                if (page->m_header->m_parent != ITZAM_NULL_REF)
                    free_page(page);
                
                page = next_page;
            }
        }
    }
    
    return success;
}

bool itzam_btree_find_before(itzam_btree * btree, const void * key, void * returned_key) {
    bool result = false;
    
    if ((btree != NULL) && (key != NULL)) {
        itzam_datafile_mutex_lock(btree->m_datafile);
        result = search_before(btree,key,returned_key);
        itzam_datafile_mutex_unlock(btree->m_datafile);
    }
    else
        default_error_handler("itzam_btree_find",ITZAM_ERROR_INVALID_DATAFILE_OBJECT);
    
    return result;
}

static bool search_after(itzam_btree * btree, const void * key, void * result) {
    int index;
    bool success = false;
    
    // start at root
    itzam_btree_page * page = &btree->m_root;
    
    while (true)  {
        index = 0;
        
        // if page is empty, we didn't find the key
        if ((page == NULL) || (page->m_header->m_key_count == 0))
            return false;
        else {
            // loop through keys
            while (index < page->m_header->m_key_count) {
                int comp = btree->m_key_comparator(key,(const void *)(page->m_keys + index * btree->m_header->m_sizeof_key));
                
                if (comp < 0) {
                    // key < page key
                    memcpy(result,(const void *)(page->m_keys + index * btree->m_header->m_sizeof_key),btree->m_header->m_sizeof_key);
                    success = true;
                    break;
                }
                else if (comp == 0) {
                    memcpy(result,(const void *)(page->m_keys + index * btree->m_header->m_sizeof_key),btree->m_header->m_sizeof_key);
                    return true;
                }
                else if (comp > 0)
                    // key > page_key
                    ++index;
            }
            
            // if we're in a leaf, we're done
            if (page->m_links[index] == ITZAM_NULL_REF)
                return success;
            else {
                // read and search next page
                itzam_btree_page * next_page = read_page(btree,page->m_links[index]);
                
                if (page->m_header->m_parent != ITZAM_NULL_REF)
                    free_page(page);
                
                page = next_page;
            }
        }
    }
    
    return success;
}

bool itzam_btree_find_after(itzam_btree * btree, const void * key, void * returned_key) {
    bool result = false;

    if ((btree != NULL) && (key != NULL)) {
        itzam_datafile_mutex_lock(btree->m_datafile);
        result = search_after(btree,key,returned_key);
        itzam_datafile_mutex_unlock(btree->m_datafile);
    }
    else
        default_error_handler("itzam_btree_find",ITZAM_ERROR_INVALID_DATAFILE_OBJECT);

    return result;
}

// promote key by creating new root
static void promote_root(itzam_btree * btree, itzam_byte * key, itzam_btree_page * page_after) {
    // create a new root page
    itzam_btree_page * new_root   = alloc_page(btree);
    itzam_btree_page * new_before = dupe_page(btree, &btree->m_root);

    // add key and links to root
    memcpy(new_root->m_keys,key,btree->m_header->m_sizeof_key);
    new_root->m_links[0] = new_before->m_header->m_where;
    new_root->m_links[1] = page_after->m_header->m_where;
    new_root->m_header->m_key_count = 1;

    // write new root to tree, and make it actual root internally
    write_page(btree,new_root);
    set_root(btree,new_root);

    // update children
    new_before->m_header->m_parent = new_root->m_header->m_where;
    page_after->m_header->m_parent = new_root->m_header->m_where;

    write_page(btree,new_before);
    write_page(btree,page_after);

    free_page(new_before);
    free_page(new_root);
}

// promote key into parent
static void promote_internal(itzam_btree * btree, itzam_btree_page * page_insert, itzam_byte * key, itzam_ref link) {
    if (page_insert->m_header->m_key_count == btree->m_header->m_order) {
        int nt  = 0;
        int ni  = 0;
        int insert_index = 0;

        itzam_btree_page * page_sibling;
        itzam_btree_page * child;

        // temporary array
        itzam_byte * temp_keys  = (itzam_byte *)malloc(btree->m_header->m_sizeof_key * (btree->m_header->m_order + 1));
        itzam_ref * temp_links = (itzam_ref *)malloc(sizeof(itzam_ref) * (btree->m_header->m_order + 2));

        if ((temp_keys == NULL) || (temp_links == NULL))
            btree->m_datafile->m_error_handler("promote_internal", ITZAM_ERROR_MALLOC);

        temp_links[0] = page_insert->m_links[0];

        // find insertion point
        while ((insert_index < page_insert->m_header->m_key_count) && (btree->m_key_comparator(key,(const void *)(page_insert->m_keys + insert_index * btree->m_header->m_sizeof_key)) >= 0))
            ++insert_index;

        // store new info
        memcpy(temp_keys + insert_index * btree->m_header->m_sizeof_key, key, btree->m_header->m_sizeof_key);
        temp_links[insert_index + 1] = link;

        // copy existing keys
        while (ni < btree->m_header->m_order) {
            if (ni == insert_index)
                ++nt;

            memcpy(temp_keys + nt * btree->m_header->m_sizeof_key, page_insert->m_keys + ni * btree->m_header->m_sizeof_key, btree->m_header->m_sizeof_key);
            temp_links[nt + 1] = page_insert->m_links[ni + 1];

            ++ni;
            ++nt;
        }

        // generate a new leaf node
        page_sibling = alloc_page(btree);
        page_sibling->m_header->m_parent = page_insert->m_header->m_parent;

        // clear key counts
        page_insert->m_header->m_key_count = 0;
        page_sibling->m_header->m_key_count = 0;
        page_insert->m_links[0] = temp_links[0];

        // copy keys from temp to pages
        for (ni = 0; ni < btree->m_min_keys; ++ni) {
            memcpy(page_insert->m_keys + ni * btree->m_header->m_sizeof_key, temp_keys + ni * btree->m_header->m_sizeof_key, btree->m_header->m_sizeof_key);
            page_insert->m_links[ni + 1] = temp_links[ni + 1];
            ++page_insert->m_header->m_key_count;
        }

        page_sibling->m_links[0] = temp_links[btree->m_min_keys + 1];

        for (ni = btree->m_min_keys + 1; ni <= btree->m_header->m_order; ++ni) {
            memcpy(page_sibling->m_keys + (ni - 1 - btree->m_min_keys) * btree->m_header->m_sizeof_key, temp_keys + ni * btree->m_header->m_sizeof_key, btree->m_header->m_sizeof_key);
            page_sibling->m_links[ni - btree->m_min_keys]    = temp_links[ni + 1];
            ++page_sibling->m_header->m_key_count;
        }

        // replace unused entries with null
        for (ni = btree->m_min_keys; ni < btree->m_header->m_order; ++ni) {
            memset(page_insert->m_keys + ni * btree->m_header->m_sizeof_key, 0, btree->m_header->m_sizeof_key);
            page_insert->m_links[ni + 1] = ITZAM_NULL_REF;
        }

        // write pages
        write_page(btree,page_insert);
        write_page(btree,page_sibling);

        if (page_insert->m_header->m_parent == ITZAM_NULL_REF)
            set_root(btree,page_insert);

        // update parent links in child nodes
        for (ni = 0; ni <= page_sibling->m_header->m_key_count; ++ni) {
            child = read_page(btree,page_sibling->m_links[ni]);

            if (child != NULL) {
                child->m_header->m_parent = page_sibling->m_header->m_where;
                write_page(btree,child);
            }
            else
                btree->m_datafile->m_error_handler("promote_internal",ITZAM_ERROR_PAGE_NOT_FOUND);

            free_page(child);
        }

        // promote key and pointer
        if (page_insert->m_header->m_parent == ITZAM_NULL_REF) {
            // create a new root
            promote_root(btree, temp_keys + btree->m_min_keys * btree->m_header->m_sizeof_key, page_sibling);
        }
        else {
            // read parent and promote key
            itzam_btree_page * parent_page = read_page(btree,page_insert->m_header->m_parent);
            promote_internal(btree, parent_page, temp_keys + btree->m_min_keys * btree->m_header->m_sizeof_key, page_sibling->m_header->m_where);
            free_page(parent_page);
        }

        // release resources
        free_page(page_sibling);
        free(temp_keys);
        free(temp_links);
    }
    else {
        int n, insert_index = 0;

        // find insertion point
        while ((insert_index < page_insert->m_header->m_key_count) && (btree->m_key_comparator(key,(void *)(page_insert->m_keys + insert_index * btree->m_header->m_sizeof_key)) >= 0))
            ++insert_index;

        // shift keys right
        for (n = page_insert->m_header->m_key_count; n > insert_index; --n) {
            memcpy(page_insert->m_keys + n * btree->m_header->m_sizeof_key, page_insert->m_keys + (n - 1) * btree->m_header->m_sizeof_key, btree->m_header->m_sizeof_key);
            page_insert->m_links[n + 1] = page_insert->m_links[n];
        }

        memcpy(page_insert->m_keys + insert_index * btree->m_header->m_sizeof_key, key, btree->m_header->m_sizeof_key);
        page_insert->m_links[insert_index + 1] = link;

        ++page_insert->m_header->m_key_count;

        // write updated page
        write_page(btree,page_insert);

        if (page_insert->m_header->m_parent == ITZAM_NULL_REF)
            set_root(btree, page_insert);
    }
}

static void write_key(itzam_btree * btree, search_result * insert_info, const itzam_byte * key) {
    itzam_btree_page * page_sibling;
    itzam_btree_page * page_parent;

    // check to see if page is full
    if (insert_info->m_page->m_header->m_key_count == btree->m_header->m_order) {
        int nt, ni;

        // temporary array to store new items
        itzam_byte * temp_keys = (itzam_byte *)malloc(btree->m_header->m_sizeof_key * (btree->m_header->m_order + 1));
        memcpy(temp_keys + insert_info->m_index * btree->m_header->m_sizeof_key, key, btree->m_header->m_sizeof_key);

        // copy entries from insertion page to temps
        nt = 0;
        ni = 0;

        while (ni < btree->m_header->m_order) {
            // skip over inserted data
            if (ni == insert_info->m_index)
                ++nt;

            // copy data
            memcpy(temp_keys + nt * btree->m_header->m_sizeof_key, insert_info->m_page->m_keys + ni * btree->m_header->m_sizeof_key, btree->m_header->m_sizeof_key);

            // next one
            ++ni;
            ++nt;
        }

        // create a new leaf
        page_sibling = alloc_page(btree);
        page_sibling->m_header->m_parent = insert_info->m_page->m_header->m_parent;

        // clear key counts
        insert_info->m_page->m_header->m_key_count = 0;
        page_sibling->m_header->m_key_count        = 0;

        // copy keys from temp to pages
        for (ni = 0; ni < btree->m_min_keys; ++ni) {
            memcpy(insert_info->m_page->m_keys + ni * btree->m_header->m_sizeof_key, temp_keys + ni * btree->m_header->m_sizeof_key, btree->m_header->m_sizeof_key);
            ++insert_info->m_page->m_header->m_key_count;
        }

        for (ni = btree->m_min_keys + 1; ni <= btree->m_header->m_order; ++ni) {
            memcpy(page_sibling->m_keys + (ni - 1 - btree->m_min_keys) * btree->m_header->m_sizeof_key, temp_keys + ni * btree->m_header->m_sizeof_key, btree->m_header->m_sizeof_key);
            ++page_sibling->m_header->m_key_count;
        }

        // replace remaining entries with null
        for (ni = btree->m_min_keys; ni < btree->m_header->m_order; ++ni)
            memset(insert_info->m_page->m_keys + ni * btree->m_header->m_sizeof_key,0,btree->m_header->m_sizeof_key);

        // write pages
        write_page(btree,insert_info->m_page);
        write_page(btree,page_sibling);

        // promote key and its pointer
        if (insert_info->m_page->m_header->m_parent == ITZAM_NULL_REF) {
            // creating a new root page
            promote_root(btree, temp_keys + btree->m_min_keys * btree->m_header->m_sizeof_key, page_sibling);
        }
        else {
            // read parent
            page_parent = read_page(btree,insert_info->m_page->m_header->m_parent);

            // promote key into parent page
            promote_internal(btree, page_parent, temp_keys + btree->m_min_keys * btree->m_header->m_sizeof_key, page_sibling->m_header->m_where);

            free_page(page_parent);
        }

        // release sibling page         
        if (page_sibling->m_header->m_parent != ITZAM_NULL_REF)
            free_page(page_sibling);

        free(temp_keys);
    }
    else {
        int n;

        // move keys to make room for new one
        for (n = insert_info->m_page->m_header->m_key_count; n > insert_info->m_index; --n)
            memcpy(insert_info->m_page->m_keys + n * btree->m_header->m_sizeof_key, insert_info->m_page->m_keys + (n - 1) * btree->m_header->m_sizeof_key, btree->m_header->m_sizeof_key);

        memcpy(insert_info->m_page->m_keys + insert_info->m_index * btree->m_header->m_sizeof_key, key, btree->m_header->m_sizeof_key);
        ++insert_info->m_page->m_header->m_key_count;

        // write updated page
        write_page(btree,insert_info->m_page);
    }
}

itzam_state itzam_btree_insert(itzam_btree * btree, const void * key)
{
    itzam_state result = ITZAM_FAILED;
    search_result insert_info;

    if ((btree != NULL) && (key != NULL) && (btree->m_cursor_count == 0)) {
        itzam_datafile_mutex_lock(btree->m_datafile);

        if (!btree->m_datafile->m_read_only) {
            search(btree,key,&insert_info);

            if (!insert_info.m_found) {
                write_key(btree,&insert_info,(const itzam_byte *)key);
                ++btree->m_header->m_count;
                ++btree->m_header->m_ticker;

                result = update_header(btree);
            }
            else
                result = ITZAM_DUPLICATE;

            if (insert_info.m_page->m_header->m_parent != ITZAM_NULL_REF)
                free_page(insert_info.m_page);
        }
        else
            result = ITZAM_READ_ONLY;

        itzam_datafile_mutex_unlock(btree->m_datafile);
    }

    //return result;
    return result;
}

static void redistribute(itzam_btree * btree, int index, itzam_btree_page * page_before, itzam_btree_page * page_parent, itzam_btree_page * page_after) {
    if ((btree != NULL) && (page_before != NULL) && (page_parent != NULL) && (page_after != NULL)) {
       // check for leaf page
       
        if (page_before->m_links[0] == ITZAM_NULL_REF) {
            if (page_before->m_header->m_key_count > page_after->m_header->m_key_count) {
                int n;

                // move a key from page_before to page_after
                for (n = page_after->m_header->m_key_count; n > 0; --n)
                    memcpy(page_after->m_keys + n * btree->m_header->m_sizeof_key, page_after->m_keys + (n - 1) * btree->m_header->m_sizeof_key, btree->m_header->m_sizeof_key);

                // store parent separator in page_after
                memcpy(page_after->m_keys, page_parent->m_keys + index * btree->m_header->m_sizeof_key, btree->m_header->m_sizeof_key);

                // increment page_after key count
                ++page_after->m_header->m_key_count;

                // decrement page_before key count
                --page_before->m_header->m_key_count;

                // move last key in page_before to page_parent as separator
                memcpy(page_parent->m_keys + index * btree->m_header->m_sizeof_key, page_before->m_keys + page_before->m_header->m_key_count * btree->m_header->m_sizeof_key, btree->m_header->m_sizeof_key);

                // clear last key in page_before
                memset(page_before->m_keys + page_before->m_header->m_key_count * btree->m_header->m_sizeof_key, 0, btree->m_header->m_sizeof_key);
            }
            else {
                int n;

                // add parent key to lesser page
                memcpy(page_before->m_keys + page_before->m_header->m_key_count * btree->m_header->m_sizeof_key, page_parent->m_keys + index * btree->m_header->m_sizeof_key, btree->m_header->m_sizeof_key);

                // increment page_before key count
                ++page_before->m_header->m_key_count;

                // move first key in page_after to page_parent as separator
                memcpy(page_parent->m_keys + index * btree->m_header->m_sizeof_key, page_after->m_keys, btree->m_header->m_sizeof_key);

                // decrement page_after key count
                --page_after->m_header->m_key_count;

                // move a key from page_after to page_before
                for (n = 0; n < page_after->m_header->m_key_count; ++n)
                    memcpy(page_after->m_keys + n * btree->m_header->m_sizeof_key, page_after->m_keys + (n + 1) * btree->m_header->m_sizeof_key, btree->m_header->m_sizeof_key);

                // clear last key in page_after
                memset(page_after->m_keys + n * btree->m_header->m_sizeof_key, 0, btree->m_header->m_sizeof_key);
            }
        }
        else {
            itzam_btree_page * page_child;

            if (page_before->m_header->m_key_count > page_after->m_header->m_key_count) {
                int n;

                // move a key from page_before to page_after
                for (n = page_after->m_header->m_key_count; n > 0; --n) {
                    memcpy(page_after->m_keys + n * btree->m_header->m_sizeof_key, page_after->m_keys + (n - 1) * btree->m_header->m_sizeof_key, btree->m_header->m_sizeof_key);
                    page_after->m_links[n + 1] = page_after->m_links[n];
                }

                page_after->m_links[1] = page_after->m_links[0];

                // store page_parent separator key in page_after
                memcpy(page_after->m_keys, page_parent->m_keys + index * btree->m_header->m_sizeof_key, btree->m_header->m_sizeof_key);
                page_after->m_links[0] = page_before->m_links[page_before->m_header->m_key_count];

                // update child link
                page_child = read_page(btree,page_after->m_links[0]);

                if (page_child != NULL) {
                    page_child->m_header->m_parent = page_after->m_header->m_where;
                    write_page(btree,page_child);
                    free_page(page_child);
                }
                else
                    btree->m_datafile->m_error_handler("redistribute",ITZAM_ERROR_PAGE_NOT_FOUND);

                // increment page_after key count
                ++page_after->m_header->m_key_count;

                // decrement page_before key count
                --page_before->m_header->m_key_count;

                // move last key in page_before to page_parent as separator
                memcpy(page_parent->m_keys + index * btree->m_header->m_sizeof_key, page_before->m_keys + page_before->m_header->m_key_count * btree->m_header->m_sizeof_key, btree->m_header->m_sizeof_key);

                // clear last key in page_before
                memset(page_before->m_keys + page_before->m_header->m_key_count * btree->m_header->m_sizeof_key, 0, btree->m_header->m_sizeof_key);
                page_before->m_links[page_before->m_header->m_key_count + 1] = ITZAM_NULL_REF;
            }
            else {
                int n;

                // store page_parent separator key in page_before
                memcpy(page_before->m_keys + page_before->m_header->m_key_count * btree->m_header->m_sizeof_key, page_parent->m_keys + index * btree->m_header->m_sizeof_key, btree->m_header->m_sizeof_key);
                page_before->m_links[page_before->m_header->m_key_count + 1] = page_after->m_links[0];

                // update child link
                page_child = read_page(btree,page_after->m_links[0]);

                if (page_child != NULL) {
                    page_child->m_header->m_parent = page_before->m_header->m_where;
                    write_page(btree,page_child);
                    free_page(page_child);
                }
                else
                    btree->m_datafile->m_error_handler("redistribute",ITZAM_ERROR_PAGE_NOT_FOUND);

                // increment page_before key count
                ++page_before->m_header->m_key_count;

                // move last key in page_after to page_parent as separator
                memcpy(page_parent->m_keys + index * btree->m_header->m_sizeof_key, page_after->m_keys, btree->m_header->m_sizeof_key);

                // decrement page_after key count
                --page_after->m_header->m_key_count;

                // move a key from page_after to page_before
                for (n = 0; n < page_after->m_header->m_key_count; ++n) {
                    memcpy(page_after->m_keys + n * btree->m_header->m_sizeof_key, page_after->m_keys + (n + 1) * btree->m_header->m_sizeof_key, btree->m_header->m_sizeof_key);
                    page_after->m_links[n] = page_after->m_links[n + 1];
                }

                page_after->m_links[n] = page_after->m_links[n + 1];

                // clear last key in page_after
                memset(page_after->m_keys + n * btree->m_header->m_sizeof_key, 0, btree->m_header->m_sizeof_key);
                page_after->m_links[n + 1] = ITZAM_NULL_REF;
            }
        }

        // write updated pages
        write_page(btree,page_before);
        write_page(btree,page_after);
        write_page(btree,page_parent);

        if(page_parent->m_header->m_parent == ITZAM_NULL_REF)
            set_root(btree, page_parent);
    }
}

static void adjust_tree(itzam_btree * btree, itzam_btree_page * page);

static void concatenate(itzam_btree * btree, int index, itzam_btree_page * page_before, itzam_btree_page * page_parent, itzam_btree_page * page_after) {
    int n, n2;

    // move separator key from page_parent into page_before
    memcpy(page_before->m_keys + page_before->m_header->m_key_count * btree->m_header->m_sizeof_key, page_parent->m_keys + index * btree->m_header->m_sizeof_key, btree->m_header->m_sizeof_key);
    page_before->m_links[page_before->m_header->m_key_count + 1] = page_after->m_links[0];

    // increment page_before key count
    ++page_before->m_header->m_key_count;

    // delete separator from page_parent
    --page_parent->m_header->m_key_count;

    for (n = index; n < page_parent->m_header->m_key_count; ++n) {
        memcpy(page_parent->m_keys + n * btree->m_header->m_sizeof_key, page_parent->m_keys + (n + 1) * btree->m_header->m_sizeof_key, btree->m_header->m_sizeof_key);
        page_parent->m_links[n + 1] = page_parent->m_links[n + 2];
    }

    // clear unused key from parent
    memset(page_parent->m_keys + n * btree->m_header->m_sizeof_key, 0, btree->m_header->m_sizeof_key);
    page_parent->m_links[n + 1] = ITZAM_NULL_REF;

    // copy keys from page_after to page_before
    n2 = 0;
    n = page_before->m_header->m_key_count;

    // combine pages
    while (n2 < page_after->m_header->m_key_count) {
        ++page_before->m_header->m_key_count;
        memcpy(page_before->m_keys + n * btree->m_header->m_sizeof_key, page_after->m_keys + n2 * btree->m_header->m_sizeof_key, btree->m_header->m_sizeof_key);
        page_before->m_links[n + 1] = page_after->m_links[n2 + 1];
        ++n2;
        ++n;
    }

    // delete page_after
    itzam_datafile_seek(btree->m_datafile,page_after->m_header->m_where);
    itzam_datafile_remove(btree->m_datafile);

    // is this an inner page?
    if (page_before->m_links[0] != ITZAM_NULL_REF) {
        // adjust child pointers
        itzam_btree_page * page_child;

        for (n = 0; n <= page_before->m_header->m_key_count; ++n) {
            // read child
            page_child = read_page(btree,page_before->m_links[n]);

            // make sure the child was actually read
             
            if (page_child != NULL) {
                page_child->m_header->m_parent = page_before->m_header->m_where;
                write_page(btree,page_child);
                free_page(page_child);
            }
            else
                btree->m_datafile->m_error_handler("concatenate",ITZAM_ERROR_PAGE_NOT_FOUND);
        }
    }

    // write page_before and parent
    if (page_parent->m_header->m_key_count == 0) {
        // update before page with old parent's parent
        page_before->m_header->m_parent = page_parent->m_header->m_parent;
        write_page(btree,page_before);

        if (page_before->m_header->m_parent == ITZAM_NULL_REF) {
            // update the header if this is the new root
            set_root(btree,page_before);
        }
        else {
            // read parents's parent
            itzam_btree_page * parents_parent = read_page(btree,page_parent->m_header->m_parent);

            // find parent reference and replace with before page
            if (parents_parent != NULL) {
                for (n = 0; n < btree->m_links_size; ++n) {
                    if (parents_parent->m_links[n] == page_parent->m_header->m_where) {
                        parents_parent->m_links[n] = page_before->m_header->m_where;
                        write_page(btree,parents_parent);
                    }
                }

                free_page(parents_parent);
            }
        }

        // remove empty parent page
        itzam_datafile_seek(btree->m_datafile,page_parent->m_header->m_where);
        itzam_datafile_remove(btree->m_datafile);
    }
    else {
        // write pages
        write_page(btree,page_parent);
        write_page(btree,page_before);

        // reset root page if needed
        if (page_parent->m_header->m_parent == ITZAM_NULL_REF)
            set_root(btree,page_parent);

        // if parent is too small, adjust
        if (page_parent->m_header->m_key_count < btree->m_min_keys)
            adjust_tree(btree,page_parent);
    }
}

static void adjust_tree(itzam_btree * btree, itzam_btree_page * page) {
    if ((btree != NULL) && (page != NULL) && (page->m_header->m_parent != ITZAM_NULL_REF)) {
        // get parent page
        itzam_btree_page * page_parent = read_page(btree,page->m_header->m_parent);

        if (page_parent != NULL) {
            itzam_btree_page * page_sibling_after  = NULL;
            itzam_btree_page * page_sibling_before = NULL;

            // find pointer to page
            int n = 0;

            while (page_parent->m_links[n] != page->m_header->m_where)
                ++n;

            // read sibling pages
            if (n < page_parent->m_header->m_key_count)
                page_sibling_after = read_page(btree,page_parent->m_links[n+1]);

            if (n > 0)
                page_sibling_before = read_page(btree,page_parent->m_links[n-1]);

            // figure out what to do
            if (page_sibling_before != NULL) {
                --n;

                if (page_sibling_before->m_header->m_key_count > btree->m_min_keys)
                    redistribute(btree,n,page_sibling_before,page_parent,page);
                else
                    concatenate(btree,n,page_sibling_before,page_parent,page);
            }
            else {
                if (page_sibling_after != NULL) {
                    if (page_sibling_after->m_header->m_key_count > btree->m_min_keys)
                        redistribute(btree,n,page,page_parent,page_sibling_after);
                    else
                        concatenate(btree,n,page,page_parent,page_sibling_after);
                }
            }

            if (page_sibling_before != NULL)
                free_page(page_sibling_before);

            if (page_sibling_after != NULL)
                free_page(page_sibling_after);

            if (page_parent->m_header->m_parent != ITZAM_NULL_REF)
                free_page(page_parent);
        }
    }
}

itzam_state itzam_btree_remove(itzam_btree * btree, const void * key) {
    itzam_state result = ITZAM_FAILED;
    search_result remove_info;

    if ((btree != NULL) && (key != NULL) && (btree->m_cursor_count == 0)) {
        itzam_datafile_mutex_lock(btree->m_datafile);

        if (btree->m_datafile->m_read_only)
            result = ITZAM_READ_ONLY;
        else {
            search(btree,key,&remove_info);

            if (remove_info.m_found) {
                // is this a leaf node?
                if (remove_info.m_page->m_links[0] == ITZAM_NULL_REF) {
                    int n;

                    // removing key from leaf
                    --remove_info.m_page->m_header->m_key_count;

                    // slide keys left over removed one
                    for (n = remove_info.m_index; n < remove_info.m_page->m_header->m_key_count; ++n)
                        memcpy(remove_info.m_page->m_keys + n * btree->m_header->m_sizeof_key, remove_info.m_page->m_keys + (n + 1) * btree->m_header->m_sizeof_key, btree->m_header->m_sizeof_key);

                    memset( remove_info.m_page->m_keys + remove_info.m_page->m_header->m_key_count * btree->m_header->m_sizeof_key, 0, btree->m_header->m_sizeof_key);

                    // save page
                    write_page(btree,remove_info.m_page);

                    // adjust the tree, if needed
                    if (remove_info.m_page->m_header->m_key_count < btree->m_min_keys)
                        adjust_tree(btree,remove_info.m_page);

                    result = ITZAM_OKAY;
                }
                else { // removing from an internal page 
                    // get the successor page
                    itzam_btree_page * page_successor = read_page(btree,remove_info.m_page->m_links[remove_info.m_index + 1]);

                    if (page_successor != NULL) {
                        int n;

                        while (page_successor->m_links[0] != ITZAM_NULL_REF) {
                            itzam_btree_page * next_successor = read_page(btree,page_successor->m_links[0]);

                            // check for null page in case of corrupted index
                            if (next_successor != NULL) {
                                free_page(page_successor);
                                page_successor = next_successor;
                            }
                            else
                                btree->m_datafile->m_error_handler("itzam_btree_remove",ITZAM_ERROR_PAGE_NOT_FOUND);
                        }

                        // first key is the "swapee"
                        memcpy(remove_info.m_page->m_keys + remove_info.m_index * btree->m_header->m_sizeof_key, page_successor->m_keys, btree->m_header->m_sizeof_key);

                        // remove swapped key from successor page
                       --page_successor->m_header->m_key_count;

                        for (n = 0; n < page_successor->m_header->m_key_count; ++n)
                            memcpy(page_successor->m_keys + n * btree->m_header->m_sizeof_key, page_successor->m_keys + (n + 1) * btree->m_header->m_sizeof_key, btree->m_header->m_sizeof_key);

                        memset(page_successor->m_keys + page_successor->m_header->m_key_count * btree->m_header->m_sizeof_key, 0, btree->m_header->m_sizeof_key);

                        // write modified pages
                        write_page(btree,remove_info.m_page);
                        write_page(btree,page_successor);

                        // adjust tree for leaf node
                        if (page_successor->m_header->m_key_count < btree->m_min_keys)
                            adjust_tree(btree,page_successor);

                        result = ITZAM_OKAY;

                        free_page(page_successor);
                    }
                }

                // decrement number of records in file
                --btree->m_header->m_count;
                update_header(btree);
            }
            else
                result = ITZAM_NOT_FOUND;

            if (remove_info.m_page->m_header->m_parent != ITZAM_NULL_REF)
                free_page(remove_info.m_page);
        }

        itzam_datafile_mutex_unlock(btree->m_datafile);
    }

    return result;
}

uint16_t itzam_btree_cursor_count(itzam_btree * btree) {
    uint16_t result = 0;

    if (btree != NULL) {
        itzam_datafile_mutex_lock(btree->m_datafile);
        result = btree->m_cursor_count;
        itzam_datafile_mutex_unlock(btree->m_datafile);
    }

    return result;
}

itzam_state itzam_btree_transaction_start(itzam_btree * btree) {
    itzam_state result = ITZAM_FAILED;

    if (btree != NULL) {
        itzam_datafile_mutex_lock(btree->m_datafile);
        result = itzam_datafile_transaction_start(btree->m_datafile);

        if (result == ITZAM_OKAY)
            btree->m_saved_header = itzam_datafile_write(btree->m_datafile->m_tran_file, btree->m_header, sizeof(itzam_btree_header), ITZAM_NULL_REF);
    }

    return result;
}

itzam_state itzam_btree_transaction_commit(itzam_btree * btree) {
    itzam_state result = ITZAM_FAILED;

    if (btree != NULL) {
        result = itzam_datafile_transaction_commit(btree->m_datafile);
        itzam_datafile_mutex_unlock(btree->m_datafile);
    }

    return result;
}

itzam_state itzam_btree_transaction_rollback(itzam_btree * btree) {
    itzam_state result = ITZAM_FAILED;
    itzam_btree_page * old_root;

    if (btree != NULL) {
        // turn off transaction processing so we can restore
        btree->m_datafile->m_in_transaction = false;
        itzam_datafile_seek(btree->m_datafile->m_tran_file, btree->m_saved_header);
        itzam_datafile_read(btree->m_datafile->m_tran_file, btree->m_header, sizeof(itzam_btree_header));
        update_header(btree);

        // turn transaction processing on again
        btree->m_datafile->m_in_transaction = true;
        result = itzam_datafile_transaction_rollback(btree->m_datafile);

        // restore the root
        old_root = read_page(btree,btree->m_header->m_root_where);
        //free(btree->m_root.m_data);
        memcpy(btree->m_root_data, old_root->m_data, btree->m_header->m_sizeof_page);

        // done here
        itzam_datafile_mutex_unlock(btree->m_datafile);
    }

    return result;
}

//-- B-tree cursor functions

static bool reset_cursor(itzam_btree_cursor * cursor) {
    bool result = false;
    bool looking = true;

    // read root 
    itzam_btree_page * next_page = NULL;
    itzam_btree_page * page = &cursor->m_btree->m_root;

    // follow the tree to the first key in the sequence 
    cursor->m_page = NULL;
    cursor->m_index = 0;

    while (looking && (page != NULL)) {
        if (page->m_header->m_key_count > 0) {
            if (page->m_links[0] == ITZAM_NULL_REF) {
                // found the first key 
                cursor->m_page = page;
                cursor->m_index = 0;
                result = true;
                looking = false;
            }
            else {
				// move to next page 
                next_page = read_page(cursor->m_btree,page->m_links[0]);

                if (page->m_header->m_parent != ITZAM_NULL_REF)
                    free_page(page);

                page = next_page;
            }
        }
        else {
            free_page(page);
            cursor->m_page = NULL;
            cursor->m_index = 0;
			looking = false;
        }
    }

    return result;
}

static bool cursor_search_before(itzam_btree_cursor * cursor, const void * key)
{
    int index;
    itzam_btree * btree = cursor->m_btree;
    bool success = false;
    
    // start at root
    itzam_btree_page * page = &btree->m_root;
    itzam_btree_page * next_page = NULL;
    
    while (true) {
        index = 0;
        
        // if page is empty, we didn't find the key
        if ((page == NULL) || (page->m_header->m_key_count == 0))
            return false;
        else {
            // loop through keys
            while (index < page->m_header->m_key_count) {
                int comp = btree->m_key_comparator(key,(const void *)(page->m_keys + index * btree->m_header->m_sizeof_key));
                
                if (comp < 0) // key < page key
                    break;
                else if (comp == 0) {
                    cursor->m_page = page;
                    cursor->m_index = index;
                    return true;
                }
                else if (comp > 0) { // key > page_key
                    cursor->m_page = page;
                    cursor->m_index = index;
                    success = true;
                    ++index;
                }
            }
            
            // if we're in a leaf, we're done
            if (page->m_links[index] == ITZAM_NULL_REF)
                return success;
            else {
                // move to next page 
                next_page = read_page(btree,page->m_links[index]);
                
                if (page->m_header->m_parent != ITZAM_NULL_REF)
                    free_page(page);
                
                page = next_page;
            }
        }
    }
    
    return success;
}

static bool cursor_search_after(itzam_btree_cursor * cursor, const void * key) {
    int index;
    itzam_btree * btree = cursor->m_btree;
    bool success = false;
    
    // start at root
    itzam_btree_page * page = &btree->m_root;
    itzam_btree_page * next_page = NULL;
    
    while (true) {
        index = 0;
        
        // if page is empty, we didn't find the key
        if ((page == NULL) || (page->m_header->m_key_count == 0))
            return false;
        else {
            // loop through keys
            while (index < page->m_header->m_key_count) {
                int comp = btree->m_key_comparator(key,(const void *)(page->m_keys + index * btree->m_header->m_sizeof_key));
                
                if (comp < 0) { // key < page key
                    cursor->m_page = page;
                    cursor->m_index = index;
                    success = true;
                    break;
                }
                else if (comp == 0) {
                    cursor->m_page = page;
                    cursor->m_index = index;
                    return true;
                }
                else if (comp > 0) // key > page_key
                    ++index;
            }
            
            // if we're in a leaf, we're done
            if (page->m_links[index] == ITZAM_NULL_REF)
                return success;
            else {
                // move to next page 
                next_page = read_page(btree,page->m_links[index]);
                page = next_page;
            }
        }
    }
    
    return success;
}

static bool set_cursor(itzam_btree_cursor * cursor, const void * key, bool before) {
    // follow the tree to the first key in the sequence 
    cursor->m_page = NULL;
    cursor->m_index = 0;
    
    // perform search 
    if (before == true)
        return cursor_search_before(cursor,key);
    else
        return cursor_search_after(cursor,key);
}

itzam_state itzam_btree_cursor_create(itzam_btree_cursor * cursor, itzam_btree * btree) {
    itzam_state result = ITZAM_FAILED;

    if ((cursor != NULL) && (btree != NULL)) {
        // keep reference to target tree 
        cursor->m_btree = btree;

        // set cursor to first index key 
        if (reset_cursor(cursor)) {
            // increment count of cursors in btree 
            ++cursor->m_btree->m_cursor_count;
            result = ITZAM_OKAY;
        }
    }

    return result;
}

itzam_state itzam_btree_cursor_create_at(itzam_btree_cursor * cursor, itzam_btree * btree, const void * key, bool before) {
    itzam_state result = ITZAM_FAILED;
    
    if ((cursor != NULL) && (btree != NULL)) {
        // keep reference to target tree 
        cursor->m_btree = btree;
        
        // set cursor to first index key 
        if (set_cursor(cursor, key, before)) {
            // increment count of cursors in btree 
            ++cursor->m_btree->m_cursor_count;
            result = ITZAM_OKAY;
        }
    }
    
    return result;
}

bool itzam_btree_cursor_valid(itzam_btree_cursor * cursor) {
    return (cursor->m_page != NULL);
}

itzam_state itzam_btree_cursor_free(itzam_btree_cursor * cursor) {
    itzam_state result = ITZAM_FAILED;

    if ((cursor != NULL) && (cursor->m_page != NULL)) {
        // decrement btree cursor count 
        if (cursor->m_btree->m_cursor_count > 0) {
            --cursor->m_btree->m_cursor_count;
            result = ITZAM_OKAY;
        }
        else
            cursor->m_btree->m_datafile->m_error_handler("itzam_btree_cursor_free",ITZAM_ERROR_CURSOR_COUNT);
    }

    return result;
}

bool itzam_btree_cursor_prev(itzam_btree_cursor * cursor) {
	itzam_ref target = ITZAM_NULL_REF;
    bool result = true;
	bool backtrack = false;

    if ((cursor != NULL) && (cursor->m_page != NULL)) {
		while (true) {
			if (cursor->m_page->m_links[cursor->m_index] == ITZAM_NULL_REF) { // are we in a leaf
				if (cursor->m_index == 0) {
					if (cursor->m_page->m_header->m_parent == ITZAM_NULL_REF)
						return false;

					loop1: // yes, there's a goto in here - oh the horror, I'm not ashamed
					// go back to origin page, find where this key is, back up, and continue
					target = cursor->m_page->m_header->m_where;

					cursor->m_page = read_page(cursor->m_btree, cursor->m_page->m_header->m_parent);
					cursor->m_index = 0;

 					while (cursor->m_page->m_links[cursor->m_index] != target)
						++cursor->m_index;

					if ((cursor->m_index == 0) && (cursor->m_page->m_header->m_parent == ITZAM_NULL_REF))
						return false;
					else 
						if (cursor->m_index == 0)
							goto loop1;
						else {
							--cursor->m_index;
							return true;
						}
				}
				else {
					--cursor->m_index;
					return true;
				}
			}
			else {
				cursor->m_page = read_page(cursor->m_btree, cursor->m_page->m_links[cursor->m_index]);

				while (cursor->m_page->m_links[cursor->m_page->m_header->m_key_count] != ITZAM_NULL_REF)
					cursor->m_page = read_page(cursor->m_btree, cursor->m_page->m_links[cursor->m_page->m_header->m_key_count]);

				cursor->m_index = cursor->m_page->m_header->m_key_count - 1;
				break;
			}
		}
	}

    return result;
}

bool itzam_btree_cursor_next(itzam_btree_cursor * cursor) {
	itzam_ref target = ITZAM_NULL_REF;
    bool result = true;
	bool backtrack = false;

    if ((cursor != NULL) && (cursor->m_page != NULL)) {
		++cursor->m_index;

		// at end of page?
		while (true) {
			if (cursor->m_index >= cursor->m_page->m_header->m_key_count) {
				if (backtrack) {
					if (cursor->m_page->m_header->m_parent == ITZAM_NULL_REF) {
						result = false;
						break;
					}

					if (cursor->m_index == cursor->m_page->m_header->m_key_count) {
						// move back to parent
						backtrack = true;
						target = cursor->m_page->m_header->m_where;

						cursor->m_index = 0;
						cursor->m_page = read_page(cursor->m_btree, cursor->m_page->m_header->m_parent);

 						while (cursor->m_page->m_links[cursor->m_index] != target)
							++cursor->m_index;

						if (cursor->m_index == cursor->m_page->m_header->m_key_count) {
							if (cursor->m_page->m_header->m_parent == ITZAM_NULL_REF) {
								result = false;
								break;
							}
							else
								continue;
						}
						else
							break;
					}
				}

				if (cursor->m_page->m_links[cursor->m_index] == ITZAM_NULL_REF) {
					if (cursor->m_page->m_header->m_parent == ITZAM_NULL_REF) {
						result = false;
						break;
					}

					// move back to parent
					backtrack = true;
					target = cursor->m_page->m_header->m_where;

					cursor->m_page = read_page(cursor->m_btree, cursor->m_page->m_header->m_parent);
					cursor->m_index = 0;

 					while (cursor->m_page->m_links[cursor->m_index] != target)
						++cursor->m_index;

					if (cursor->m_index != cursor->m_page->m_header->m_key_count)
						break;
				}
				else {
					// move to child
					cursor->m_page = read_page(cursor->m_btree, cursor->m_page->m_links[cursor->m_index]);
					cursor->m_index = 0;

					if (cursor->m_page->m_links[cursor->m_index] == ITZAM_NULL_REF)
						break;
				}
			}
			else {
				if (cursor->m_page->m_links[cursor->m_index] == ITZAM_NULL_REF) 
					// nothing more, we're fine
					break;
				else {
					// move to child
					cursor->m_page = read_page(cursor->m_btree, cursor->m_page->m_links[cursor->m_index]);
					cursor->m_index = 0;

					if (cursor->m_page->m_links[cursor->m_index] == ITZAM_NULL_REF)
						break;
				}
			}
		}
    }

    return result;
}

bool itzam_btree_cursor_reset(itzam_btree_cursor * cursor) {
    return reset_cursor(cursor);
}

itzam_state itzam_btree_cursor_read(itzam_btree_cursor * cursor, void * returned_key) {
    itzam_state result = ITZAM_NOT_FOUND;

    if ((cursor != NULL) && (returned_key != NULL) && (cursor->m_page != NULL)) {
        memcpy(returned_key, cursor->m_page->m_keys + cursor->m_index * cursor->m_btree->m_header->m_sizeof_key, cursor->m_btree->m_header->m_sizeof_key);
        result = ITZAM_OKAY;
    }

    return result;
}

//-- diagnostic routines
 
void itzam_btree_dump_page(itzam_btree * btree, itzam_btree_page * page, itzam_ref parent_ref) {
    int n;

    fprintf(stdout,"-->> PAGE DUMP\n");
    fprintf(stdout,"m_data *      = %8p, m_header * = %8p\n",(void *)page->m_data,(void *)page->m_header);
    fprintf(stdout,"m_where       = %ld (0x%lx)\n",(long)page->m_header->m_where,(unsigned long)page->m_header->m_where);

    if (page->m_header->m_parent == parent_ref)
        fprintf(stdout,"m_parent      = %ld (0x%lx)\n",(long)page->m_header->m_parent,(unsigned long)page->m_header->m_parent);
    else
        fprintf(stdout,"m_parent      = %ld (0x%lx) INVALID PARENT\n",(long)page->m_header->m_parent,(unsigned long)page->m_header->m_parent);

    fprintf(stdout,"m_key_count   = %u (0x%x)\n",page->m_header->m_key_count,page->m_header->m_key_count);

    for (n = 0; n < btree->m_header->m_order; ++n) {
        fprintf(stdout,"m_links[%04d] = %ld (0x%lx)\n",n,(long)page->m_links[n],(unsigned long)page->m_links[n]);
        fprintf(stdout,"m_keys [%04d] = %ld (0x%lx)\n",n,(*((long *)(page->m_keys + sizeof(int32_t) * n))),*((uint32_t *)(page->m_keys + sizeof(int32_t) * n)));
    }

    fprintf(stdout,"m_links[%04d] = %ld (0x%lx)\n",n,(long)page->m_links[n],(unsigned long)page->m_links[n]);
    fprintf(stdout,"<<--\n");
}

static void dump_btree_recursive(itzam_btree * btree, itzam_btree_page * page, itzam_ref parent_ref) {
    int n;

    itzam_btree_dump_page(btree,page,parent_ref);

    if (page->m_header->m_key_count > 0) {
        for (n = 0; n <= page->m_header->m_key_count; ++n) {
            if (page->m_links[n] != ITZAM_NULL_REF) {
                itzam_btree_page * temp = read_page(btree,page->m_links[n]);
                dump_btree_recursive(btree,temp,page->m_header->m_where);
                free_page(temp);
            }
        }
    }
}

static void print_header(const itzam_btree * btree, FILE * output) {
    fprintf(output,"btree header information\n------------------------------------------------------------\n");
    fprintf(output,"btree *                = 0x%8p\n",(void *)btree);
    fprintf(output,"m_header.m_sizeof_page = %d\n",btree->m_header->m_sizeof_page);
    fprintf(output,"m_header.m_order       = %d\n",btree->m_header->m_order);
    fprintf(output,"m_header.m_count       = %lu\n",(unsigned long)btree->m_header->m_count);
    fprintf(output,"m_header.m_ticker      = %lu\n", (unsigned long)btree->m_header->m_ticker);
    fprintf(output,"m_header.m_where       = %ld (0x%lx)\n",(long)btree->m_header->m_where,(long)btree->m_header->m_where);
    fprintf(output,"m_header.m_root_where  = %ld (0x%lx)\n",(long)btree->m_header->m_root_where,(long)btree->m_header->m_root_where);
    fprintf(output,"m_links_size           = %d\n",btree->m_links_size);
    fprintf(output,"m_min_keys             = %d\n",btree->m_min_keys);
    fprintf(output,"------------------------------------------------------------\n");
}

void itzam_btree_dump_btree(itzam_btree * btree) {
    itzam_btree_page * root;

    fprintf(stdout,"---> BEGIN BTREE ANALYSIS\n");
    print_header(btree,stdout);

    root = read_page(btree, btree->m_header->m_root_where);
    dump_btree_recursive(btree,root,ITZAM_NULL_REF);

    fprintf(stdout,"<--- END BTREE DUMP\n");
}

struct anal_data {
    FILE *      output;
    uint32_t    total_avail;
    uint32_t    total_inuse;
    double      balance;
    double      branch_count;
    uint32_t    page_count;
    uint32_t    height;
};

static void analyze_page(itzam_btree * btree, itzam_btree_page * page, struct anal_data * data) {
    data->total_avail += btree->m_header->m_order;
    data->total_inuse += page->m_header->m_key_count;

    if (page->m_links[0] != ITZAM_NULL_REF) {
        data->balance += (double)page->m_header->m_key_count / (double)btree->m_header->m_order;
        data->branch_count += 1.0;
    }

    fprintf(data->output,".");
}

static void analyze_btree_recursive(itzam_btree * btree, itzam_btree_page * page, struct anal_data * data) {
    int n;

    analyze_page(btree,page,data);

    if (page->m_header->m_key_count > 0) {
        for (n = 0; n <= page->m_header->m_key_count; ++n) {
            if (page->m_links[n] != ITZAM_NULL_REF) {
                itzam_btree_page * temp = read_page(btree,page->m_links[n]);
                ++data->page_count;
                analyze_btree_recursive(btree,temp,data);
            }
        }
    }
}

static void analyze_btree_height(itzam_btree * btree, struct anal_data * data) {
    itzam_btree_page * page = read_page(btree,btree->m_header->m_root_where);

    data->height = 1;

    while (page->m_links[0] != ITZAM_NULL_REF) {
        ++data->height;
        page = read_page(btree,page->m_links[0]);
    }
}

void itzam_btree_analyze(itzam_btree * btree, FILE * output) {
    struct anal_data data;
    itzam_btree_page * root;

    data.output       = output;
    data.total_avail  = 0;
    data.total_inuse  = 0;
    data.balance      = 0.0;
    data.branch_count = 1;
    data.page_count   = 1;
    data.height       = 0;

    fprintf(output,"---> BEGIN BTREE ANALYSIS\n\n");
    print_header(btree,output);

    fprintf(output,"\nanalyzing\n");
    root = read_page(btree, btree->m_header->m_root_where);
    analyze_btree_recursive(btree,root,&data);
    analyze_btree_height(btree,&data);
    fprintf(output,"\ncomplete\n");

    fprintf(output,"\noverall statistics:\n");
    fprintf(output,"----------------------------------------------------------------------\n");
    fprintf(output," key slots available = %8u\n",data.total_avail);
    fprintf(output,"    key slots in use = %8u\n",data.total_inuse);
    fprintf(output,"    total # of pages = %8u\n",data.page_count);
    fprintf(output,"         keys in use = %8.2f%%\n",100.0 * (double)data.total_inuse / (double)data.total_avail);
    fprintf(output,"overall tree balance = %8.2f%% (50%% or better is good in large trees)\n",100.0 * data.balance / (double)data.branch_count);
    fprintf(output,"         tree height = %8u\n",data.height);
    fprintf(output,"----------------------------------------------------------------------\n");
    fprintf(output,"<--- END BTREE ANALYSIS\n\n");
}
