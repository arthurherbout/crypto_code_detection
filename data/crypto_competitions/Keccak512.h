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

#ifndef KECCAK512_H
#define KECCAK512_H

// ** Thread-safe implementation

// ** Keccak hashing
// ** 512bit hash

#include "Keccak512_data.h"

extern	void Keccak512_init(KECCAK512_DATA *keccak);
extern	void Keccak512_data(KECCAK512_DATA *keccak,const BYTE *buffer,DWORD len);
extern	void Keccak512_finalize(KECCAK512_DATA *keccak,BYTE *hash);

#endif
