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
#include <errno.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#if defined(ITZAM_LINUX)
static pthread_mutex_t global_mutex = PTHREAD_MUTEX_INITIALIZER;
#endif

//-- deleted record list management
 
static itzam_state read_dellist(itzam_datafile * datafile) {
    itzam_state result = ITZAM_FAILED;

    if (datafile->m_shared->m_header.m_dellist_ref != ITZAM_NULL_REF)
    {
        if (-1 != itzam_file_seek(datafile->m_file,datafile->m_shared->m_header.m_dellist_ref + sizeof(itzam_record_header),ITZAM_SEEK_BEGIN))
        {
            // read the header
            if (itzam_file_read(datafile->m_file,&datafile->m_dellist_header,sizeof(itzam_dellist_header)))
            {
                itzam_int size = sizeof(itzam_dellist_entry) * datafile->m_dellist_header.m_table_size;

                if (datafile->m_dellist != NULL)
                    free(datafile->m_dellist);

                datafile->m_dellist = (itzam_dellist_entry *)malloc(size);

                if (datafile->m_dellist != NULL)
                {
                    if (itzam_file_read(datafile->m_file,datafile->m_dellist,size))
                        result = ITZAM_OKAY;
                }
                else
                    datafile->m_error_handler("read_dellist",ITZAM_ERROR_MALLOC);
            }
        }

        if (result != ITZAM_OKAY)
            datafile->m_error_handler("read_dellist",ITZAM_ERROR_DELLIST_NOT_READ);
    }
    else
        datafile->m_dellist = NULL;

    return result;
}

static itzam_state write_dellist(itzam_datafile * datafile, bool has_grown) {
    itzam_int   size   = sizeof(itzam_dellist_entry) * datafile->m_dellist_header.m_table_size;
    itzam_state result = ITZAM_FAILED;
    itzam_record_header header;

    if (has_grown) {
        // remove old record if there was one
        if (datafile->m_shared->m_header.m_dellist_ref != ITZAM_NULL_REF) {
            if (-1 != itzam_file_seek(datafile->m_file,datafile->m_shared->m_header.m_dellist_ref,ITZAM_SEEK_BEGIN)) {
                // read the header
                if (itzam_file_read(datafile->m_file,&header,sizeof(header))) {
                    // change record header; make this record the head of the deleted list
                    header.m_flags &= ~ITZAM_RECORD_IN_USE;
                    header.m_flags &= ~ITZAM_RECORD_DELLIST;

                    // move to beginning of record header again and rewrite it
                    itzam_file_seek(datafile->m_file,datafile->m_shared->m_header.m_dellist_ref,ITZAM_SEEK_BEGIN);

                    if (itzam_file_write(datafile->m_file,&header,sizeof(header)))
                        result = ITZAM_OKAY;
                }
            }
        }

        // explicitly append; we can't use write because it might try to change the deleted list while we're saving it
        if (-1 != itzam_file_seek(datafile->m_file,0,ITZAM_SEEK_END)) {
            datafile->m_shared->m_header.m_dellist_ref = itzam_file_tell(datafile->m_file);

            if (datafile->m_shared->m_header.m_dellist_ref > 0) {
                // write new record header
                itzam_record_header rec_head;

                rec_head.m_signature = ITZAM_RECORD_SIGNATURE;
                rec_head.m_flags     = ITZAM_RECORD_IN_USE | ITZAM_RECORD_DELLIST;
                rec_head.m_length    = sizeof(itzam_dellist_header) + size;
                rec_head.m_rec_len  = rec_head.m_length;

                if (itzam_file_write(datafile->m_file,&rec_head,sizeof(itzam_record_header))) {
                    // write the list header
                    if (itzam_file_write(datafile->m_file,&datafile->m_dellist_header,sizeof(itzam_dellist_header))) {
                        // write the list
                        if (itzam_file_write(datafile->m_file,datafile->m_dellist,size)) {
                            // update the header with the pointer to the new deleted record list
                            if (-1 != itzam_file_seek(datafile->m_file,0,ITZAM_SEEK_BEGIN)) {
                                if (itzam_file_write(datafile->m_file,&datafile->m_shared->m_header,sizeof(itzam_datafile_header)))
                                    result = ITZAM_OKAY;
                            }
                        }
                    }
                }
            }
        }
    }
    else
    {
        if (-1 != itzam_file_seek(datafile->m_file,datafile->m_shared->m_header.m_dellist_ref + sizeof(itzam_record_header),ITZAM_SEEK_BEGIN)) {
            // write the header
            if (itzam_file_write(datafile->m_file,&datafile->m_dellist_header,sizeof(itzam_dellist_header))) {
                // write the list
                if (itzam_file_write(datafile->m_file,datafile->m_dellist,size))
                    result = ITZAM_OKAY;
            }
        }
    }

    if (result != ITZAM_OKAY)
        datafile->m_error_handler("write_dellist",ITZAM_ERROR_DELLIST_NOT_WRITTEN);

    return result;
}

//-- utilities
 
static char * get_tranfile_name(const char * filename){
    static const char * tran_name_mask = "%s.itzamtran";
    char * result = (char *)malloc(strlen(tran_name_mask) + strlen(filename) + 1);
    sprintf(result,tran_name_mask,filename);
    return result;
}

bool itzam_datafile_exists(const char * filename) {
    struct stat info;
    return (0 == stat(filename, &info));
}

static char * get_shared_name(const char * mask, const char * filename) {
    char * result = (char *)malloc(strlen(mask) + strlen(filename) + 1);

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

    sprintf(result, mask, norm);

    free(norm);

    return result;
}

#if defined(ITZAM_LINUX)
static const char * shared_mask = "/%s_ItzamSharedDatafile";
#else
static const char * shared_mask = "Global\\%s_ItzamSharedDatafile";
static const char * mutex_mask = "Global\\%s_ItzamMutex";
#endif

//-- datafile functions
 
itzam_state itzam_datafile_create(itzam_datafile * datafile, const char * filename) {
    itzam_datafile_header header;
    itzam_state result = ITZAM_FAILED;
    bool creator;

    #if defined(ITZAM_LINUX)
        pthread_mutexattr_t attr;
        pthread_mutex_lock(&global_mutex);
    #else
        char * mutex_name;
    #endif

    // verify arguments before proceeding
    if (datafile != NULL) {
        // fill header
        header.m_signature        = ITZAM_DATAFILE_SIGNATURE;
        header.m_version          = ITZAM_DATAFILE_VERSION;
        header.m_dellist_ref      = ITZAM_NULL_REF;
        header.m_schema_ref       = ITZAM_NULL_REF;
        header.m_index_list_ref   = ITZAM_NULL_REF;
        header.m_transaction_tail = ITZAM_NULL_REF;

        // fill remaining datafile members
        #if defined(ITZAM_LINUX)
            datafile->m_file    = -1;
            datafile->m_shmem   = -1;
        #else
            datafile->m_file    = NULL;
            datafile->m_shmem   = NULL;
        #endif

        datafile->m_filename        = strdup(filename);
        datafile->m_dellist         = NULL;
        datafile->m_tran_file       = NULL;
        datafile->m_tran_replacing  = false;
        datafile->m_shared          = NULL;
        datafile->m_is_open         = false;
        datafile->m_file_locked     = false;
        datafile->m_in_transaction  = false;
        datafile->m_error_handler   = default_error_handler;

        #if defined(ITZAM_LINUX)
            memset(&datafile->m_file_lock,0,sizeof(struct flock));
        #endif

        // generate transaction file name
        datafile->m_tran_file_name = get_tranfile_name(filename);;

        // generate shared memory for header
        datafile->m_shmem_name = get_shared_name(shared_mask, filename);

        // create OS-level file
        datafile->m_file = itzam_file_create(filename);

        if (ITZAM_GOOD_FILE(datafile->m_file)) {
            // write header
            if (itzam_file_write(datafile->m_file,&header,sizeof(itzam_datafile_header))) {
                itzam_file_commit(datafile->m_file);

                // generate shared memory and fill it
                datafile->m_shmem = itzam_shmem_obtain(datafile->m_shmem_name, sizeof(itzam_datafile_shared), &creator);
                datafile->m_shared = (itzam_datafile_shared *)itzam_shmem_getptr(datafile->m_shmem, sizeof(itzam_datafile_shared));
                datafile->m_shared->m_count = 1;

                // obtain mutex
                #if defined(ITZAM_LINUX)
                    pthread_mutexattr_init(&attr);
                    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
                    pthread_mutex_init(&datafile->m_shared->m_mutex, &attr);
                #else
                    mutex_name = get_shared_name(mutex_mask,filename);

                    datafile->m_mutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, (LPCSTR)mutex_name);

                    if (datafile->m_mutex == NULL)
                        datafile->m_mutex = CreateMutex(NULL, FALSE, (LPCSTR)mutex_name);
                #endif

                datafile->m_read_only = false; // can't be read only durign creation 
                memcpy(&datafile->m_shared->m_header, &header, sizeof(itzam_datafile_header));

                result = ITZAM_OKAY;
            }
            else
                default_error_handler("itzam_datafile_create",ITZAM_ERROR_WRITE_FAILED);
        }
        else
            default_error_handler("itzam_datafile_create",ITZAM_ERROR_FILE_CREATE);

        datafile->m_is_open = (result == ITZAM_OKAY);
    }
    else
        default_error_handler("itzam_datafile_create",ITZAM_ERROR_INVALID_DATAFILE_OBJECT);

    #if defined(ITZAM_LINUX)
        pthread_mutex_unlock(&global_mutex);
    #else
        free(mutex_name);
    #endif

    return result;
}

itzam_state itzam_datafile_open(itzam_datafile * datafile, const char * filename, bool recover, bool read_only) {
    bool have_header = false;
    bool creator = false;
    itzam_state result = ITZAM_FAILED;

    #if defined(ITZAM_LINUX)
        pthread_mutexattr_t attr;
        pthread_mutex_lock(&global_mutex);
    #else
        char * mutex_name;
    #endif

    // verify arguments before proceeding
    if (datafile != NULL) {
        // set default error handler
        datafile->m_error_handler  = default_error_handler;
        datafile->m_tran_file      = NULL;
        datafile->m_tran_replacing = false;
        datafile->m_file_locked    = false;
        datafile->m_is_open        = false;
        datafile->m_dellist        = NULL;
        datafile->m_in_transaction = false;

        #if defined(ITZAM_LINUX)
            memset(&datafile->m_file_lock,0,sizeof(struct flock));
        #endif

        // assumes that datafile is a clean structure (i.e., it isn't open already)
        datafile->m_file = itzam_file_open(filename);

        if (ITZAM_GOOD_FILE(datafile->m_file)) {
            datafile->m_is_open = true;

            // get tranfile name
            datafile->m_tran_file_name = get_tranfile_name(filename);;

            // get shared memory
            datafile->m_shmem_name = get_shared_name(shared_mask, filename);
            datafile->m_shmem = itzam_shmem_obtain(datafile->m_shmem_name, sizeof(itzam_datafile_shared), &creator);
            datafile->m_shared = (itzam_datafile_shared *)itzam_shmem_getptr(datafile->m_shmem, sizeof(itzam_datafile_shared));

            #if defined(ITZAM_LINUX)
                if (creator) {
                    pthread_mutexattr_init(&attr);
                    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
                    pthread_mutex_init(&datafile->m_shared->m_mutex, &attr);
                }
            #else
                mutex_name = get_shared_name(mutex_mask,filename);

                datafile->m_mutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, (LPCSTR)mutex_name);

                if (datafile->m_mutex == NULL)
                    datafile->m_mutex = CreateMutex(NULL, FALSE, (LPCSTR)mutex_name);
            #endif

            datafile->m_read_only  = read_only;

            if (creator)
                datafile->m_shared->m_count = 1;
            else
                datafile->m_shared->m_count += 1;

            // read the header
            if (creator)
                have_header = itzam_file_read(datafile->m_file, &datafile->m_shared->m_header, sizeof(itzam_datafile_header));
            else
                have_header = true;

            if (have_header) {
                // verify signature and version
                if (datafile->m_shared->m_header.m_signature == ITZAM_DATAFILE_SIGNATURE) {
                    if (datafile->m_shared->m_header.m_version == ITZAM_DATAFILE_VERSION) {
                        // read deleted list, if any
                        if (datafile->m_shared->m_header.m_dellist_ref != ITZAM_NULL_REF)
                            result = read_dellist(datafile);
                        else
                            result = ITZAM_OKAY;

                        // if we have a dangling transaction, roll it back
                        if (recover && (datafile->m_shared->m_header.m_transaction_tail != ITZAM_NULL_REF))
                            itzam_datafile_transaction_rollback(datafile);
                    }
                    else
                        datafile->m_error_handler("itzam_datafile_open",ITZAM_ERROR_VERSION);
                }
                else
                    datafile->m_error_handler("itzam_datafile_open",ITZAM_ERROR_SIGNATURE);
            }
            else
                datafile->m_error_handler("itzam_datafile_open",ITZAM_ERROR_OPEN_FAILED);
        }
    }
    else
        default_error_handler("itzam_datafile_open",ITZAM_ERROR_INVALID_DATAFILE_OBJECT);

    #if defined(ITZAM_LINUX)
        pthread_mutex_unlock(&global_mutex);
    #else
        free(mutex_name);
    #endif

    return result;
}

itzam_state itzam_datafile_close(itzam_datafile * datafile) {
    itzam_state result = ITZAM_FAILED;
    bool last_owner = false;

    #if defined(ITZAM_LINUX)
        pthread_mutex_lock(&global_mutex);
    #endif

    if (datafile != NULL) {
        itzam_datafile_mutex_lock(datafile);
        datafile->m_shared->m_count -= 1;
        itzam_datafile_mutex_unlock(datafile);

        last_owner = (datafile->m_shared->m_count <= 0) ? true : false;

        if (last_owner)
            #if defined(ITZAM_LINUX)
                pthread_mutex_destroy(&datafile->m_shared->m_mutex);
            #else
                CloseHandle(datafile->m_mutex);
            #endif

        if (datafile->m_dellist != NULL) {
            free(datafile->m_dellist);
            datafile->m_dellist = NULL;
        }

        free(datafile->m_tran_file_name);

        itzam_shmem_freeptr(datafile->m_shared, sizeof(itzam_datafile_shared));

        if (last_owner)
            itzam_shmem_close(datafile->m_shmem, datafile->m_shmem_name);

        free(datafile->m_shmem_name);

        if (itzam_file_close(datafile->m_file)) {
            datafile->m_is_open = false;
            result = ITZAM_OKAY;
        }
        else
            datafile->m_error_handler("itzam_datafile_close",ITZAM_ERROR_CLOSE_FAILED);
    }
    else
        default_error_handler("itzam_datafile_close",ITZAM_ERROR_INVALID_DATAFILE_OBJECT);

    #if defined(ITZAM_LINUX)
        pthread_mutex_unlock(&global_mutex);
    #endif

    return result;
}

void itzam_datafile_mutex_lock(itzam_datafile * datafile) {
    #if defined(ITZAM_LINUX)
        pthread_mutex_lock(&datafile->m_shared->m_mutex);
    #else
        WaitForSingleObject(datafile->m_mutex, INFINITE);
    #endif
}

void itzam_datafile_mutex_unlock(itzam_datafile * datafile) {
    #if defined(ITZAM_LINUX)
        pthread_mutex_unlock(&datafile->m_shared->m_mutex);
    #else
        ReleaseMutex(datafile->m_mutex);
    #endif
}

bool itzam_datafile_file_lock(itzam_datafile * datafile) {
    bool result = false;

    if ((datafile != NULL) && (datafile->m_is_open)) {
        if (datafile->m_file_locked)
            result = true;
        else {
            #if defined(ITZAM_LINUX)
                memset(&datafile->m_file_lock,0,sizeof(struct flock));
                datafile->m_file_lock.l_type = F_WRLCK;
                result = (bool)fcntl(datafile->m_file,F_SETLKW,&datafile->m_file_lock);
            #else
                result = (bool)LockFile(datafile->m_file, 0, 0, 0xFFFFFFFF, 0xFFFFFFFF);
            #endif
        }

        datafile->m_file_locked = result;
    }
    else
        default_error_handler("itzam_datafile_file_lock",ITZAM_ERROR_INVALID_DATAFILE_OBJECT);

    return result;
}

bool itzam_datafile_file_unlock(itzam_datafile * datafile) {
    bool result = false;

    if ((datafile != NULL) && (datafile->m_is_open)) {
        if (datafile->m_file_locked)  {
            #if defined(ITZAM_LINUX)
                fsync(datafile->m_file);
                datafile->m_file_lock.l_type = F_UNLCK;
                result = (bool)fcntl(datafile->m_file,F_SETLKW,&datafile->m_file_lock);
            #else
                result = (bool)UnlockFile(datafile->m_file, 0, 0, 0xFFFFFFFF, 0xFFFFFFFF);
            #endif
        }
        else
            result = true; // file wasn't locked, so it *is* unlocked... 

        if (result == true)
            datafile->m_file_locked = false;
    }
    else
        default_error_handler("itzam_datafile_file_unlock",ITZAM_ERROR_INVALID_DATAFILE_OBJECT);

    return result;
}

bool itzam_datafile_is_open(itzam_datafile * datafile) {
    bool result = false;

    if (datafile != NULL) {
        itzam_datafile_mutex_lock(datafile);
        result = datafile->m_is_open;
        itzam_datafile_mutex_unlock(datafile);
    }
    else
        default_error_handler("itzam_datafile_is_open",ITZAM_ERROR_INVALID_DATAFILE_OBJECT);

    return result;
}

float itzam_datafile_get_version(itzam_datafile * datafile) {
    float result = 0.0F;

    if (datafile != NULL) {
        itzam_datafile_mutex_lock(datafile);

        result = (double)((datafile->m_shared->m_header.m_version & 0x00FF0000) >> 16)
               + ((datafile->m_shared->m_header.m_version & 0x0000FF00) >> 8) / 100.0
               + ((datafile->m_shared->m_header.m_version & 0x000000FF)) / 10000.0;

        itzam_datafile_mutex_unlock(datafile);
    }
    else
        default_error_handler("itzam_datafile_get_version",ITZAM_ERROR_INVALID_DATAFILE_OBJECT);

    return result;
}

void itzam_datafile_set_error_handler(itzam_datafile * datafile, itzam_error_handler * error_handler) {
    if (datafile != NULL) {
        itzam_datafile_mutex_lock(datafile);

        if (error_handler == NULL)
            datafile->m_error_handler = default_error_handler;
        else
            datafile->m_error_handler = error_handler;

        itzam_datafile_mutex_unlock(datafile);
    }
    else
        default_error_handler("itzam_datafile_set_error_handler",ITZAM_ERROR_INVALID_DATAFILE_OBJECT);
}

itzam_ref itzam_datafile_tell(itzam_datafile * datafile) {
    itzam_ref where = -1;

    if ((datafile != NULL) && (datafile->m_is_open)) {
        itzam_datafile_mutex_lock(datafile);

        where = (itzam_ref)itzam_file_tell(datafile->m_file);

        itzam_datafile_mutex_unlock(datafile);
    }
    else
        default_error_handler("itzam_datafile_tell",ITZAM_ERROR_INVALID_DATAFILE_OBJECT);

    return where;
}

itzam_state itzam_datafile_seek(itzam_datafile * datafile, itzam_ref  pos) {
    itzam_state result = ITZAM_FAILED;

    if ((datafile != NULL) && (datafile->m_is_open)) {
        itzam_datafile_mutex_lock(datafile);

        // seek to requested position
         
        if (-1 != itzam_file_seek(datafile->m_file,(long)pos,ITZAM_SEEK_BEGIN))
            result = ITZAM_OKAY;
        else
            datafile->m_error_handler("itzam_datafile_seek",ITZAM_ERROR_SEEK_FAILED);

        itzam_datafile_mutex_unlock(datafile);
    }
    else
        default_error_handler("itzam_datafile_seek",ITZAM_ERROR_INVALID_DATAFILE_OBJECT);

    return result;
}

itzam_state itzam_datafile_rewind(itzam_datafile * datafile) {
    itzam_state result = ITZAM_FAILED;

    if ((datafile != NULL) && (datafile->m_is_open)) {
        itzam_datafile_mutex_lock(datafile);

        // seek to byte after first deleted marker
         
        if (-1 != itzam_file_seek(datafile->m_file,(long)sizeof(itzam_datafile_header),ITZAM_SEEK_BEGIN))
            result = ITZAM_OKAY;
        else
            datafile->m_error_handler("itzam_datafile_rewind",ITZAM_ERROR_SEEK_FAILED);

        itzam_datafile_mutex_unlock(datafile);
    }
    else
        default_error_handler("itzam_datafile_rewind",ITZAM_ERROR_INVALID_DATAFILE_OBJECT);

    return result;
}

// This function should NEVER be called by user code; it is an internal function used by indexes.
// It assumes that a returned deleted record will be used by the calling function.
 
itzam_ref itzam_datafile_get_next_open(itzam_datafile * datafile, itzam_int length) {
    itzam_int n;
    itzam_ref where = ITZAM_NULL_REF;

    if ((datafile != NULL) && (datafile->m_is_open)) {
        if (ITZAM_OKAY == read_dellist(datafile)) {
            for (n = 0; n < datafile->m_dellist_header.m_table_size; ++n) {
                if (datafile->m_dellist[n].m_where != ITZAM_NULL_REF) {
                    if (datafile->m_dellist[n].m_length == length) {
                        // save the location of the deleted record we're replacing
                        where = datafile->m_dellist[n].m_where;
                        // remove this entry from the table
                        datafile->m_dellist[n].m_where  = ITZAM_NULL_REF;
                        datafile->m_dellist[n].m_length = 0;
                        write_dellist(datafile,false);
                        break;
                    }
                }
            }
        }

        if (where == ITZAM_NULL_REF) { // no deleted records, so append
            if (-1 != itzam_file_seek(datafile->m_file,0,ITZAM_SEEK_END)) {
                where = itzam_file_tell(datafile->m_file);


                if (where < 0)
                    where = ITZAM_NULL_REF;
            }
            else
                where = ITZAM_NULL_REF;
        }
    }
    else
        default_error_handler("itzam_datafile_get_next_open",ITZAM_ERROR_INVALID_DATAFILE_OBJECT);

    return where;
}

// internal function to add an operation to the transaction list
 
static bool add_tran_op(itzam_datafile * datafile, itzam_int where, itzam_record_header * header, const void * data, itzam_op_type op_type) {
    bool result = false;
    itzam_op_header op_header;
    itzam_int op_where;

    if (datafile->m_in_transaction) {
        // initialize operation header and copy data in
        op_header.m_type         = op_type;
        op_header.m_where        = where;
        op_header.m_prev_tran    = datafile->m_shared->m_header.m_transaction_tail;
        memcpy(&op_header.m_record_header,header,sizeof(itzam_record_header));
        op_header.m_record_header.m_flags |= ITZAM_RECORD_TRAN_RECORD;
        op_header.m_record_where = ITZAM_NULL_REF;

        // turn off transactions for a moment
        datafile->m_in_transaction = false;

        // write op data to the file
        if (op_type != ITZAM_TRAN_OP_WRITE)  {
            op_header.m_record_where = itzam_datafile_get_next_open(datafile->m_tran_file,op_header.m_record_header.m_length);
            itzam_datafile_write_flags(datafile->m_tran_file,data,op_header.m_record_header.m_length,op_header.m_record_where,ITZAM_RECORD_TRAN_RECORD);
        }

        // write op header
        op_where = itzam_datafile_get_next_open(datafile->m_tran_file,sizeof(itzam_op_header));
        itzam_datafile_write_flags(datafile->m_tran_file,&op_header,sizeof(op_header),op_where,ITZAM_RECORD_TRAN_HEADER);

        // turn transactions on again
        datafile->m_in_transaction = true;

        // rewrite datafile header
        datafile->m_shared->m_header.m_transaction_tail = op_where;

        if (-1 != itzam_file_seek(datafile->m_file,0,ITZAM_SEEK_BEGIN))         {
            if (itzam_file_write(datafile->m_file,&datafile->m_shared->m_header,sizeof(itzam_datafile_header)))
                result = true;
        }
    }

    return result;
}

itzam_ref itzam_datafile_write_flags(itzam_datafile * datafile, const void * data, itzam_int length, itzam_ref where, int32_t flags) {
    itzam_record_header rec_header;

    // make sure the arguments make sense
    if ((datafile != NULL) && (data != NULL) && (length > 0)  && (datafile->m_is_open))
    {
        itzam_datafile_mutex_lock(datafile);

        if (datafile->m_read_only)
            return ITZAM_NULL_REF;
        else {
            // if we aren't told where to put the record, find a place
            if (where == ITZAM_NULL_REF)
                where = itzam_datafile_get_next_open(datafile,length);
            else {
                // are we in a transaction?
                if (datafile->m_in_transaction) {
                    // read the old record rec_header
                    if (-1 != itzam_file_seek(datafile->m_file,where,ITZAM_SEEK_BEGIN)) {
                        if (itzam_file_read(datafile->m_file,&rec_header,sizeof(itzam_record_header))) {
                            // if the record is active, then we need to save it
                            if (rec_header.m_flags & ITZAM_RECORD_IN_USE) {
                                // create a temporary buffer
                                void * op_data = malloc(rec_header.m_rec_len);

                                // read data
                                if (op_data != NULL) {
                                    // now save record information
                                    if (itzam_file_read(datafile->m_file,op_data,rec_header.m_length)) {
                                        add_tran_op(datafile,where,&rec_header,op_data,ITZAM_TRAN_OP_REMOVE);
                                        free(op_data);
                                    }
                                    else
                                        datafile->m_error_handler("itzam_datafile_write_flags (1)",ITZAM_ERROR_READ_FAILED);
                                }
                                else
                                    datafile->m_error_handler("itzam_datafile_write_flags (2)",ITZAM_ERROR_MALLOC);
                            }
                        }
                    }
                }
            }

            rec_header.m_signature = ITZAM_RECORD_SIGNATURE;
            rec_header.m_flags     = ITZAM_RECORD_IN_USE | flags;
            rec_header.m_length    = 0;
            rec_header.m_rec_len   = 0;

            // do we have a good place to write?
            if (where != ITZAM_NULL_REF) {
                // modify record rec_header
                rec_header.m_rec_len = length;
                rec_header.m_length  = length;

                // ITZAM_SEEK to beginning of new record
                if (-1 != itzam_file_seek(datafile->m_file,where,ITZAM_SEEK_BEGIN)) {
                    // write the record rec_header
                    if (itzam_file_write(datafile->m_file,&rec_header,sizeof(rec_header))) {
                        // write the record
                        if (!itzam_file_write(datafile->m_file,data,length)) {
                            datafile->m_error_handler("itzam_datafile_write_flags (4)",ITZAM_ERROR_WRITE_FAILED);
                            where = ITZAM_NULL_REF;
                        }
                    }
                    else {
                        datafile->m_error_handler("itzam_datafile_write_flags (5)", ITZAM_ERROR_WRITE_FAILED);
                        where = ITZAM_NULL_REF;
                    }
                }
                else {
                    datafile->m_error_handler("itzam_datafile_write_flags (6)",ITZAM_ERROR_SEEK_FAILED);
                    where = ITZAM_NULL_REF;
                }

                if ((where != ITZAM_NULL_REF) && (datafile->m_in_transaction))
                    add_tran_op(datafile,where,&rec_header,data,ITZAM_TRAN_OP_WRITE);
            }
        }

        itzam_datafile_mutex_unlock(datafile);
    }
    else
        default_error_handler("itzam_datafile_write_flags (7)",ITZAM_ERROR_INVALID_DATAFILE_OBJECT);

    return where;
}

itzam_ref itzam_datafile_write(itzam_datafile * datafile, const void * data, itzam_int length, itzam_ref where) {
    return itzam_datafile_write_flags(datafile, data, length, where, 0);
}

// dangerous! this function could overwrite critical file data structures
itzam_state itzam_datafile_overwrite(itzam_datafile * datafile, const void * data, itzam_int length, itzam_ref where, itzam_int offset) {
    itzam_state result = ITZAM_FAILED;
    itzam_record_header rec_header;

    if ((datafile != NULL) && (data != NULL) && (length > 0)  && (datafile->m_is_open) && (where != ITZAM_NULL_REF)) {
        if (datafile->m_read_only) {
            datafile->m_error_handler("itzam_datafile_write_flags", ITZAM_ERROR_READ_ONLY);
            return ITZAM_FAILED;
        }

        itzam_datafile_mutex_lock(datafile);

        // read header file
        if (-1 != itzam_file_seek(datafile->m_file,where,ITZAM_SEEK_BEGIN)) {
            if (!itzam_file_read(datafile->m_file,&rec_header,sizeof(itzam_record_header)))
                 datafile->m_error_handler("itzam_datafile_explicit_write (1)",ITZAM_ERROR_READ_FAILED);

            if ((0 == (rec_header.m_flags & ITZAM_RECORD_IN_USE)) || (rec_header.m_signature != ITZAM_RECORD_SIGNATURE))
                 datafile->m_error_handler("itzam_datafile_explicit_write (2)",ITZAM_ERROR_INVALID_RECORD);
        }

        // make sure we fit inside the record
        if (rec_header.m_length < (length + offset))
            return ITZAM_OVERWRITE_TOO_LONG;

        // are we in a transaction? if so, save the rec we're changing
        if (datafile->m_in_transaction) {
            // create a temporary buffer
            void * op_data = malloc(rec_header.m_rec_len);

            // read data
            if (op_data != NULL) {
                // now save record information
                if (itzam_file_read(datafile->m_file,op_data,rec_header.m_length)) {
                    add_tran_op(datafile, where, &rec_header, op_data, ITZAM_TRAN_OP_OVERWRITE);
                    free(op_data);
                }
                else
                    datafile->m_error_handler("itzam_datafile_explicit_write (2)",ITZAM_ERROR_READ_FAILED);
            }
            else
                datafile->m_error_handler("itzam_datafile_explicit_write (3)",ITZAM_ERROR_MALLOC);
        }

        // modify record at given offset
         
        if (-1 != itzam_file_seek(datafile->m_file, where + sizeof(itzam_record_header) + offset, ITZAM_SEEK_BEGIN)) {
            if (!itzam_file_write(datafile->m_file,data,length)) {
                datafile->m_error_handler("itzam_datafile_explicit_write (4)",ITZAM_ERROR_WRITE_FAILED);
                where = ITZAM_NULL_REF;
            }
        }
        else  {
            datafile->m_error_handler("itzam_datafile_explicit_write (5)",ITZAM_ERROR_SEEK_FAILED);
            where = ITZAM_NULL_REF;
        }

        itzam_datafile_mutex_unlock(datafile);

        result = ITZAM_OKAY;
    }
    else
        default_error_handler("itzam_datafile_write_flags (5)",ITZAM_ERROR_INVALID_DATAFILE_OBJECT);

    return result;
}

itzam_state itzam_datafile_read(itzam_datafile * datafile, void * record, itzam_int max_length) {
    itzam_state result = ITZAM_FAILED;
    itzam_record_header header = { 0, 0, 0, 0 };
    bool error = false;

    if ((datafile != NULL) && (record != NULL) && (max_length > 0) && (datafile->m_is_open)) {
        itzam_datafile_mutex_lock(datafile);

        while (!error)  {
            // read the record header
            if (itzam_file_read(datafile->m_file,&header,sizeof(itzam_record_header))) {
                // if the record is active, we can read it
                if ((header.m_flags & ITZAM_RECORD_IN_USE) && !(header.m_flags & ITZAM_RECORD_DELLIST))
                    break;

                // if the record signature is invalid, we have an error
                if (header.m_signature != ITZAM_RECORD_SIGNATURE) {
                    datafile->m_error_handler("itzam_datafile_read",ITZAM_ERROR_INVALID_RECORD);
                    error = true;
                }

                // move to the next record
                if (-1 == itzam_file_seek(datafile->m_file,header.m_length,ITZAM_SEEK_CURRENT)) {
                    datafile->m_error_handler("itzam_datafile_read",ITZAM_ERROR_SEEK_FAILED);
                    error = true;
                }
            }
            else {
                datafile->m_error_handler("itzam_datafile_read",ITZAM_ERROR_READ_FAILED);
                error = true;
            }
        }

        // only read actual data if no errors
        if (!error) {
            itzam_int read_len = (max_length < header.m_rec_len) ? max_length : header.m_rec_len;

            if (itzam_file_read(datafile->m_file, record, read_len)) {
                // skip any "padding" between record size and record buffer length
                if (read_len < header.m_length) {
                    if (-1 == itzam_file_seek(datafile->m_file,header.m_length - read_len,ITZAM_SEEK_CURRENT)) {
                        datafile->m_error_handler("itzam_datafile_read",ITZAM_ERROR_SEEK_FAILED);
                        error = true;
                    }
                }

                result = ITZAM_OKAY;
            }
            else  {
                datafile->m_error_handler("itzam_datafile_read",ITZAM_ERROR_READ_FAILED);
                error = true;
            }
        }

        itzam_datafile_mutex_unlock(datafile);
    }
    else
        default_error_handler("itzam_datafile_read",ITZAM_ERROR_INVALID_DATAFILE_OBJECT);

    return result;
}

itzam_state itzam_datafile_read_alloc(itzam_datafile * datafile, void ** data, itzam_int * length) {
    itzam_state result = ITZAM_FAILED;
    itzam_record_header header;

    if ((datafile != NULL) && (data != NULL) && (datafile->m_is_open)) {
        itzam_datafile_mutex_lock(datafile);

        if (itzam_file_read(datafile->m_file,&header,sizeof(header))) {
            // if the record is active, we can read it
            if (header.m_flags & ITZAM_RECORD_IN_USE) {
                // create a temporary buffer
                if (length != NULL)
                    *length = header.m_rec_len;

                *data   = malloc(header.m_rec_len);

                // read data
                if (*data != NULL) {
                    if (itzam_file_read(datafile->m_file,*data,header.m_length)) {
                        // skip any "padding" between record size and record buffer length
                        if (header.m_rec_len < header.m_length)
                            itzam_file_seek(datafile->m_file,header.m_length - header.m_rec_len,ITZAM_SEEK_CURRENT);

                        result = ITZAM_OKAY;
                    }
                    else
                        datafile->m_error_handler("1) itzam_datafile_read_alloc",ITZAM_ERROR_READ_FAILED);
                }
                else
                    datafile->m_error_handler("itzam_datafile_read_alloc",ITZAM_ERROR_MALLOC);
            }
            else
                datafile->m_error_handler("2) itzam_datafile_read_alloc",ITZAM_ERROR_READ_FAILED);
        }
        else
            datafile->m_error_handler("3) itzam_datafile_read_alloc",ITZAM_ERROR_READ_FAILED);

        itzam_datafile_mutex_unlock(datafile);
    }
    else
        default_error_handler("itzam_datafile_read_alloc",ITZAM_ERROR_INVALID_DATAFILE_OBJECT);

    return result;
}

itzam_state itzam_datafile_remove(itzam_datafile * datafile) {
    itzam_state result = ITZAM_FAILED;
    int n;
    itzam_ref where;

    if ((datafile != NULL) && (datafile->m_is_open)) {
        itzam_datafile_mutex_lock(datafile);

        if (datafile->m_read_only)
            result = ITZAM_READ_ONLY;
        else {
            // get our position
            where = itzam_file_tell(datafile->m_file);

            // make sure we're not in the file header
            if (where >= sizeof(itzam_ref)) {
                itzam_record_header header;

                // read the header
                if (itzam_file_read(datafile->m_file,&header,sizeof(header))) {
                    // only delete if it isn't already deleted
                    if (header.m_flags & ITZAM_RECORD_IN_USE) {
                        // save this record if we're in a transaction
                        if (datafile->m_in_transaction) {
                            // create a temporary buffer
                            void * op_data = malloc(header.m_rec_len);

                            // read data
                            if (op_data != NULL) {
                                // now save record information
                                if (itzam_file_read(datafile->m_file,op_data,header.m_length)) {
                                    add_tran_op(datafile,where,&header,op_data,ITZAM_TRAN_OP_REMOVE);
                                    free(op_data);
                                }
                                else
                                    datafile->m_error_handler("itzam_datafile_remove",ITZAM_ERROR_READ_FAILED);
                            }
                            else
                                datafile->m_error_handler("itzam_datafile_remove",ITZAM_ERROR_MALLOC);
                        }

                        // change record header; make this record the head of the deleted list
                        header.m_flags &= ~ITZAM_RECORD_IN_USE;

                        // move to beginning of record header
                        if (-1 != itzam_file_seek(datafile->m_file,where,ITZAM_SEEK_BEGIN)) {
                            // write revised record header
                            if (itzam_file_write(datafile->m_file,&header,sizeof(header))) {
                                // update deleted list
                                if (ITZAM_FAILED == read_dellist(datafile))  {
                                    datafile->m_dellist_header.m_table_size = ITZAM_DELLIST_BLOCK_SIZE;
                                    datafile->m_dellist = (itzam_dellist_entry *)malloc(sizeof(itzam_dellist_entry) * datafile->m_dellist_header.m_table_size);

                                    if (datafile->m_dellist != NULL) {
                                        datafile->m_dellist[0].m_where  = where;
                                        datafile->m_dellist[0].m_length = header.m_length;

                                        for (n = 1; n < datafile->m_dellist_header.m_table_size; ++n) {
                                            datafile->m_dellist[n].m_where  = ITZAM_NULL_REF;
                                            datafile->m_dellist[n].m_length = 0;
                                        }

                                        result = write_dellist(datafile,true);
                                    }
                                    else
                                        datafile->m_error_handler("itzam_datafile_remove",ITZAM_ERROR_MALLOC);
                                }
                                else {
                                    for (n = 0; n < datafile->m_dellist_header.m_table_size; ++n) {
                                        if (datafile->m_dellist[n].m_where == ITZAM_NULL_REF) {
                                            datafile->m_dellist[n].m_where  = where;
                                            datafile->m_dellist[n].m_length = header.m_length;
                                            result = write_dellist(datafile,false);
                                            break;
                                        }
                                    }

                                    // if the entry didn't fit, we have to expand it
                                    if (n == datafile->m_dellist_header.m_table_size) {
                                        // create new table
                                        itzam_int newsize = datafile->m_dellist_header.m_table_size + ITZAM_DELLIST_BLOCK_SIZE;
                                        itzam_dellist_entry * newlist = (itzam_dellist_entry *)malloc(sizeof(itzam_dellist_entry) * newsize);

                                        if (newlist != NULL) {
                                            // copy old entries; could (?should) use memcpy here I suppose
                                            for (n = 0; n < datafile->m_dellist_header.m_table_size; ++n) {
                                                newlist[n].m_where  = datafile->m_dellist[n].m_where;
                                                newlist[n].m_length = datafile->m_dellist[n].m_length;
                                            }

                                            // add the newly-deleted record
                                            newlist[n].m_where  = where;
                                            newlist[n].m_length = header.m_length;

                                            // add the old deleted list record
                                            newlist[n+1].m_where  = datafile->m_shared->m_header.m_dellist_ref;
                                            newlist[n+1].m_length = datafile->m_dellist_header.m_table_size;

                                            // initialize remaining unused new entries
                                            for (n += 2 ; n < newsize; ++n) {
                                                newlist[n].m_where  = ITZAM_NULL_REF;
                                                newlist[n].m_length = 0;
                                            }

                                            // free space used by old list
                                            free(datafile->m_dellist);

                                            // exchange new list for old
                                            datafile->m_dellist = newlist;
                                            datafile->m_dellist_header.m_table_size = newsize;

                                            // write it
                                            result = write_dellist(datafile,true);
                                        }
                                        else
                                            datafile->m_error_handler("itzam_datafile_read_alloc",ITZAM_ERROR_MALLOC);
                                    }
                                }
                            }
                            else
                                datafile->m_error_handler("itzam_datafile_remove",ITZAM_ERROR_WRITE_FAILED);
                        }
                        else
                            datafile->m_error_handler("itzam_datafile_remove",ITZAM_ERROR_SEEK_FAILED);
                    }
                    else
                        datafile->m_error_handler("itzam_datafile_remove",ITZAM_ERROR_DUPE_REMOVE);
                }
                else
                    datafile->m_error_handler("itzam_datafile_remove",ITZAM_ERROR_READ_FAILED);
            }
            else
                datafile->m_error_handler("itzam_datafile_remove",ITZAM_ERROR_TELL_FAILED);
        }

        itzam_datafile_mutex_unlock(datafile);
    }
    else
        default_error_handler("itzam_datafile_remove",ITZAM_ERROR_INVALID_DATAFILE_OBJECT);

    return result;
}

itzam_state itzam_datafile_transaction_start(itzam_datafile * datafile) {
    itzam_state result = ITZAM_FAILED;

    // only start a transaction is one isn't already in progress
    if ((datafile != NULL) && (datafile->m_is_open) && (!datafile->m_in_transaction)) {
        if (datafile->m_read_only) {
            datafile->m_error_handler("itzam_datafile_transactions_start", ITZAM_ERROR_READ_ONLY);
            return ITZAM_READ_ONLY;
        }

        itzam_datafile_mutex_lock(datafile);

        datafile->m_tran_file = (itzam_datafile *)malloc(sizeof(itzam_datafile));

        if (datafile->m_tran_file != NULL) {
            // create a datafile to contain the transaction operations
            result = itzam_datafile_create(datafile->m_tran_file,datafile->m_tran_file_name);

            if (ITZAM_OKAY == result)
                datafile->m_in_transaction = true;
            else
                default_error_handler("itzam_datafile_transaction_start",ITZAM_ERROR_INVALID_DATAFILE_OBJECT);
        }
    }
    else
        default_error_handler("itzam_datafile_transaction_start",ITZAM_ERROR_INVALID_DATAFILE_OBJECT);

    return result;
}

static void transaction_cleanup(itzam_datafile * datafile, bool rollback) {
    itzam_op_header * op_header;
    itzam_int dont_care;
    void * op_record;
    itzam_int data_len;
    itzam_int n;
    bool dummy;

    if (rollback) {
        // start at the beginning
        itzam_ref op_where = datafile->m_shared->m_header.m_transaction_tail;

        // while we have something to process
        while (op_where != ITZAM_NULL_REF) {
            // read the operation header
            itzam_datafile_seek(datafile->m_tran_file,op_where);

            // read the rolled-back record
            itzam_datafile_read_alloc(datafile->m_tran_file,(void **)(void*)&op_header,&dont_care);

            // act upon the op_record....
            switch (op_header->m_type) {
                // remove a record that was written
                case ITZAM_TRAN_OP_WRITE:
                    itzam_datafile_seek(datafile,op_header->m_where);
                    itzam_datafile_remove(datafile);
                    break;

                // replace a record that was removed
                case ITZAM_TRAN_OP_REMOVE:
                    itzam_datafile_seek(datafile->m_tran_file,op_header->m_record_where);
                    itzam_datafile_read_alloc(datafile->m_tran_file,(void **)&op_record,&data_len);
                    itzam_datafile_write(datafile,op_record,op_header->m_record_header.m_length,op_header->m_where);
                    free(op_record);

                    if (ITZAM_OKAY == read_dellist(datafile)) {
                        for (n = 0; n < datafile->m_dellist_header.m_table_size; ++n) {
                            if (datafile->m_dellist[n].m_where == op_header->m_where) {
                                // remove this entry from the table
                                datafile->m_dellist[n].m_where  = ITZAM_NULL_REF;
                                datafile->m_dellist[n].m_length = 0;
                                break;
                            }
                        }
                    }

                    write_dellist(datafile,false);

                    break;

                // restore a record that was over-written
                case ITZAM_TRAN_OP_OVERWRITE:
                    itzam_datafile_seek(datafile->m_tran_file,op_header->m_record_where);
                    itzam_datafile_read_alloc(datafile->m_tran_file,(void **)&op_record,&data_len);
                    itzam_datafile_write(datafile, op_record, op_header->m_record_header.m_length, op_header->m_where);
                    free(op_record);

                    break;
            }

            // save next operation
            op_where = op_header->m_prev_tran;

            // release memory
            free(op_header);
        }
    }

    // update the header
    datafile->m_shared->m_header.m_transaction_tail = ITZAM_NULL_REF;
    itzam_file_seek(datafile->m_file,0,ITZAM_SEEK_BEGIN);
    dummy = itzam_file_write(datafile->m_file,&datafile->m_shared->m_header,sizeof(itzam_datafile_header));

    // close and remove transaction file
    itzam_datafile_close(datafile->m_tran_file);
    itzam_file_remove(datafile->m_tran_file_name);
    free(datafile->m_tran_file);
    datafile->m_tran_file = NULL;

    itzam_datafile_mutex_unlock(datafile);
}

itzam_state itzam_datafile_transaction_commit(itzam_datafile * datafile) {
    itzam_state result = ITZAM_FAILED;

    // only commit if a transaction is active
    if ((datafile != NULL) && (datafile->m_is_open) && (datafile->m_in_transaction)) {
        // we're no longer in a transaction
        datafile->m_in_transaction = false;

        // clean up transactioon data; it's no longer needed
        transaction_cleanup(datafile,false);
        result = ITZAM_OKAY;
    }
    else
        default_error_handler("itzam_datafile_transaction_commit",ITZAM_ERROR_INVALID_DATAFILE_OBJECT);

    return result;
}

itzam_state itzam_datafile_transaction_rollback(itzam_datafile * datafile) {
    itzam_state result = ITZAM_FAILED;

    // only rollback if a transaction is active
    if ((datafile != NULL) && (datafile->m_is_open) && (datafile->m_in_transaction)) {
        // we're no longer in a transaction
        datafile->m_in_transaction = false;

        // clean up transaction data; it's no longer needed
        transaction_cleanup(datafile,true);
        result = ITZAM_OKAY;
    }
    else
        default_error_handler("itzam_datafile_transaction_rollback",ITZAM_ERROR_INVALID_DATAFILE_OBJECT);

    return result;
}
