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

#ifndef TWOFISH_H
#define TWOFISH_H

// ** Thread-safe implementation

// ** Twofish cipher
// ** 128bit block size
// ** 256bit key

#include "Twofish_data.h"

extern	void Twofish_set_key(TWOFISH_DATA *pTfd,const DWORD *in_key, const DWORD key_len);
extern	void Twofish_encrypt(const TWOFISH_DATA *pTfd,const DWORD *in_blk, DWORD *out_blk);
extern	void Twofish_decrypt(const TWOFISH_DATA *pTfd,const DWORD *in_blk, DWORD *out_blk);

#endif
