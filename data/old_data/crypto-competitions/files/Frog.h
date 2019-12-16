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

#ifndef FROG_H
#define FROG_H

// ** Thread-safe implementation

// ** Frog cipher
// ** 128bit block size
// ** 256bit key

#include "Frog_data.h"

extern	void Frog_set_key(FROG_DATA *pFd,const DWORD *in_key,const DWORD key_len);
extern	void Frog_encrypt(const FROG_DATA *pFd,const DWORD in_blk[4], DWORD out_blk[4]);
extern	void Frog_decrypt(const FROG_DATA *pFd,const DWORD in_blk[4], DWORD out_blk[4]);

#endif
