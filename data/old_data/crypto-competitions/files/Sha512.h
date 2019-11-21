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

#ifndef SHA512_H
#define SHA512_H

// ** Thread-safe implementation

// ** Sha2 hashing
// ** 512bit hash

#include "Sha512_data.h"

extern	void Sha512_init(SHA512_DATA *sha);
extern	void Sha512_data(SHA512_DATA *sha,const void *buffer,DWORD len);
extern	void Sha512_finalize(SHA512_DATA *sha,BYTE *hash);

#endif
