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

#ifndef CLEFIA_H
#define CLEFIA_H

// ** Thread-safe implementation

// ** Clefia cipher
// ** 128bit block size
// ** 256bit key

#include "Clefia_data.h"

extern	void Clefia_set_key(CLEFIA_DATA *pCd,const BYTE *skey,const int key_bitlen);
extern	void Clefia_encrypt(const CLEFIA_DATA *pCd,const BYTE *pt,BYTE *ct);
extern	void Clefia_decrypt(const CLEFIA_DATA *pCd,const BYTE *ct,BYTE *pt);

#endif
