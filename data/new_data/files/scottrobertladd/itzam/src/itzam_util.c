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
#include <sys/types.h>
#include <sys/stat.h>

#if defined(ITZAM_LINUX)
#include <sys/mman.h>
#endif

//-- default error handler, for errors outside the scope of valid datafile objects
 
void default_default_error_handler(const char * function_name, itzam_error error) {
    // default error handler displays a simple message and exits program
    fprintf(stderr,"Itzam internal error in %s: %d\n",function_name,error);
    exit(1);
}

itzam_error_handler * default_error_handler = default_default_error_handler;

void itzam_set_default_error_handler(itzam_error_handler * handler) {
    if (handler != NULL)
        default_error_handler = handler;
}

//-- shared memory

ITZAM_SHMEM_TYPE itzam_shmem_obtain(const char * name, size_t len, bool * created) {
    #if defined(ITZAM_LINUX)
        int result;

        result = shm_open(name, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);

        if (result < 0) {
            if (errno == EEXIST) {
                *created = false;

                result = shm_open(name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

                if (result < 0)
                    default_error_handler("itzam_shmem_obtain",ITZAM_ERROR_SHMEM);
            }
        }
        else {
            *created = true;

            if (ftruncate(result, len))
                default_error_handler("itzam_shmem_obtain",ITZAM_ERROR_SHMEM);
        }

        return (ITZAM_SHMEM_TYPE)result;
    #else
        HANDLE result;

        result = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, (LPCSTR)name);

        if ((result == NULL) && (len > 0)) {
            *created = true;
            result = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, (DWORD)len, (LPCTSTR)name);
        }
        else
            *created = false;

        if (result == NULL) {
            if (ERROR_ACCESS_DENIED == GetLastError())
                default_error_handler("itzam_shmem_obtain",ITZAM_ERROR_SHMEM_PRIVILEGE);
            else
                default_error_handler("itzam_shmem_obtain",ITZAM_ERROR_SHMEM);
        }

        return result;
    #endif
}

void itzam_shmem_close(ITZAM_SHMEM_TYPE shmem, const char * name) {
    #if defined(ITZAM_LINUX)
        close(shmem);
        shm_unlink(name);
    #else
        if (shmem != NULL)
            CloseHandle(shmem);
    #endif
}

void * itzam_shmem_getptr(ITZAM_SHMEM_TYPE shmem, size_t len) {
    #if defined(ITZAM_LINUX)
        void * result = mmap(NULL, len, PROT_READ | PROT_WRITE,  MAP_SHARED, shmem, 0);

        if (result == MAP_FAILED)
            default_error_handler("itzam_shmem_obtain",ITZAM_ERROR_SHMEM);

        return result;
    #else
        void * result = NULL;

        result = (void *)MapViewOfFile(shmem, FILE_MAP_ALL_ACCESS, 0, 0, len);

        if (result == NULL) {
            if (ERROR_ACCESS_DENIED == GetLastError())
                default_error_handler("itzam_shmem_obtain",ITZAM_ERROR_SHMEM_PRIVILEGE);
            else
                default_error_handler("itzam_shmem_obtain",ITZAM_ERROR_SHMEM);
        }

        return result;
    #endif
}

bool itzam_shmem_freeptr(void * shmem_ptr, size_t len) {
    #if defined(ITZAM_LINUX)
        return (bool)(0 == munmap(shmem_ptr,len));
    #else
        return (bool)UnmapViewOfFile(shmem_ptr);
    #endif
}

//-- file I/O functions

ITZAM_FILE_TYPE itzam_file_create(const char * filename) {
    #if defined(ITZAM_LINUX)
        return open(filename, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH );
    #else
        HANDLE result = NULL;

        do {
            result = CreateFile((LPCSTR)filename, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_FLAG_RANDOM_ACCESS, NULL);

            if ((result != NULL) && (result != INVALID_HANDLE_VALUE))
                break;

            if ((result == INVALID_HANDLE_VALUE) && (GetLastError() == ERROR_ACCESS_DENIED))
                Sleep(250);
            else
                break;
        }
        while (1);

        return result;
    #endif
}

ITZAM_FILE_TYPE itzam_file_open(const char * filename) {
    #if defined(ITZAM_LINUX)
        return open(filename, O_RDWR);
    #else
        return CreateFile((LPCSTR)filename, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_FLAG_RANDOM_ACCESS, NULL);
    #endif
}

bool itzam_file_close(ITZAM_FILE_TYPE file) {
    #if defined(ITZAM_LINUX)
        return (bool)(0 == close(file));
    #else
        return (bool)CloseHandle(file);
    #endif
}

bool itzam_file_read(ITZAM_FILE_TYPE file, void * data, size_t len) {
    #if defined(ITZAM_LINUX)
        return (bool)(len == read(file,data,len));
    #else
        DWORD count;
        return (bool)ReadFile(file, (LPVOID)data, (DWORD)len, &count, NULL);
    #endif
}

bool itzam_file_write(ITZAM_FILE_TYPE file, const void * data, size_t len) {
    #if defined(ITZAM_LINUX)
        return (bool)(len == write(file,data,len));
    #else
        DWORD count;
        return (bool)WriteFile(file,(LPVOID)data, (DWORD)len, &count, NULL);
    #endif
}

itzam_ref itzam_file_seek(ITZAM_FILE_TYPE file, itzam_ref pos, int mode) {
    #if defined(ITZAM_LINUX)
        return (itzam_ref)lseek(file,pos,mode);
    #else
        return (itzam_ref)SetFilePointer(file, (LONG)pos, NULL, mode);
    #endif
}

itzam_ref itzam_file_tell(ITZAM_FILE_TYPE file) {
    #if defined(ITZAM_LINUX)
        return (itzam_ref)lseek(file,0,SEEK_CUR);
    #else
        return (itzam_ref)SetFilePointer(file, 0, NULL, FILE_CURRENT);
    #endif
}

bool itzam_file_commit(ITZAM_FILE_TYPE file) {
    #if defined(ITZAM_LINUX)
        return true; // (bool)commit(file);
    #else
        return (bool)FlushFileBuffers(file);
    #endif
}

bool itzam_file_remove(const char * filename) {
    #if defined(ITZAM_LINUX)
        return (bool)remove(filename);
    #else
        return (bool)DeleteFile((LPCSTR)filename);
    #endif
}
