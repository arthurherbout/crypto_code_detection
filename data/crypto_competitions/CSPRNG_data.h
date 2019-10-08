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
// You’re free to copy, distribute and make commercial use
// of this software under the following conditions:
// * You cite the author and copyright owner: "www.embeddedsw.net"
// * You provide a link to the Homepage: "www.embeddedsw.net/libobfuscate.html"
//
///////////////////////////////////////////////

#ifndef CSPRNG_DATA_H
#define CSPRNG_DATA_H

// ** Thread-safe implementation

// ** Cryptographically Secure Pseudo Random Number Generator (by construction)
// ** Based on AES-256 + CTR

#include "CommonDef.h"
#include "MultiBase_data.h"

typedef enum {
	SHA512_HASH=0,
	GROSTL512_HASH,
	KECCAK512_HASH,
	SKEIN512_HASH
} ENUM_HASH;

#define	MAX_HASH		4

typedef struct {
	MULTI_STATIC_DATA	msd;
	BYTE				ctrBuf[DATA_BLOCK_SIZE];
	BYTE				randBuf[DATA_BLOCK_SIZE];
	DWORD				availCount;
} CSPRNG_DATA;

#endif
