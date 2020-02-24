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

#ifndef SAFERP_H
#define SAFERP_H

// ** Thread-safe implementation

// ** Safer+ cipher
// ** 128bit block size
// ** 256bit key

#include "Saferp_data.h"

extern	void Saferp_set_key(SAFERP_DATA *pSpd,const DWORD *in_key, const DWORD key_len);
extern	void Saferp_encrypt(const SAFERP_DATA *pSpd,const DWORD *in_blk, DWORD *out_blk);
extern	void Saferp_decrypt(const SAFERP_DATA *pSpd,const DWORD *in_blk, DWORD *out_blk);

#endif



