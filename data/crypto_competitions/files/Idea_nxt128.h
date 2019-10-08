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

#ifndef IDEANXT128_H
#define IDEANXT128_H

// ** Thread-safe implementation

// ** Idea-Nxt cipher
// ** 128bit block size
// ** 256bit key

#include "Idea_nxt128_data.h"

extern	void Ideanxt128_set_key(nxt128_ctx *ctx,const BYTE *key,const WORD key_len);
extern	void Ideanxt128_encrypt(const nxt128_ctx *ctx,const BYTE *in,BYTE *out);
extern	void Ideanxt128_decrypt(const nxt128_ctx *ctx,const BYTE *in,BYTE *out);

#endif
