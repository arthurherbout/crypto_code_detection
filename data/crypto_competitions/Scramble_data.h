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

#ifndef SCRAMBLE_DATA_H
#define SCRAMBLE_DATA_H

// ** Thread-safe implementation

// ** Scramble data segment
// ** Based on CSPRNG

#include "CSPRNG_data.h"

typedef struct {
	CSPRNG_DATA	cd;
	DWORD		len;
	DWORD		*list;
} SCRAMBLE_DATA;

#endif
