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

#ifndef SKEIN512_H
#define SKEIN512_H

// ** Thread-safe implementation

// ** Skein hashing
// ** 512bit hash

#include "Skein512_data.h"

extern	void Skein512_init(SKEIN512_DATA *skein);
extern	void Skein512_data(SKEIN512_DATA *skein,const BYTE *buffer,DWORD len);
extern	void Skein512_finalize(SKEIN512_DATA *skein,BYTE *hash);

#endif
