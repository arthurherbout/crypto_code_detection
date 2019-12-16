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

#ifndef MULTI_DATA_H
#define MULTI_DATA_H

// ** Thread-safe implementation

// ** Wrapping of implemented ciphers
// ** 128bit block size	(DATA_BLOCK_SIZE<<3)
// ** 2x 256bit keys	(MAX_PASSW_SIZE<<3)
// ** Cypher-Block-Chaining (CBC)

#include "CommonDef.h"
#include "MultiBase_data.h"
#include "CSPRNG_data.h"

typedef struct {
	MULTI_STATIC_DATA	msd;
	CSPRNG_DATA			cd;
	BYTE				iv[MAX_ALG][DATA_BLOCK_SIZE];
} MULTI_DATA;

#endif
