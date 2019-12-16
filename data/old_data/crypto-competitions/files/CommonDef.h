///////////////////////////////////////////////
//
// **************************
// ** ENGLISH - 10/Jul/2017 **
//
// Project: libObfuscate v2.00
//
// This software is released under:
// * LGPL 3.0: "www.gnu.org/licenses/lgpl.html"
//
// You're free to copy, distribute and make commercial use
// of this software under the following conditions:
// * You cite the author and copyright owner: "www.embeddedsw.net"
// * You provide a link to the Homepage: "www.embeddedsw.net/libobfuscate.html"
//
///////////////////////////////////////////////

#ifndef COMMON_DATA_H
#define COMMON_DATA_H

// ** Library common definitions

typedef enum {
	OBFUNC_OK=0,
	OBFUNC_OUTOFMEM,
	OBFUNC_INVALID,
	OBFUNC_STOP
} OBFUNC_RETV;

typedef	void (*perc_callback_t)(void *desc,BYTE perc);
typedef	BOOL (*test_callback_t)(void *desc);

#define	MIN_PASSW_SIZE			8
#define	MAX_PASSW_SIZE			32

#define	MAX_ALG					16

#define	DATA_BLOCK_SIZE			16

#define	IV_BLOCK_SIZE			(MAX_ALG*DATA_BLOCK_SIZE)

#define	TEST_AND_FREE(argX)	if(argX) { free(argX); argX=NULL; }

#endif
