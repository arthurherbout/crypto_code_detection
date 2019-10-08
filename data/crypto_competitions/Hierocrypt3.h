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

#ifndef HIEROCRYPT3_H
#define HIEROCRYPT3_H

// ** Thread-safe implementation

// ** Hierocrypt3 cipher
// ** 128bit block size
// ** 256bit key

#include "Hierocrypt3_data.h"

extern	void Hierocrypt3_set_key(HIEROCRYPT3_DATA *pHd,const BYTE* key);
extern	void Hierocrypt3_encrypt(const HIEROCRYPT3_DATA *pHd,const BYTE * in,BYTE *out);
extern	void Hierocrypt3_decrypt(const HIEROCRYPT3_DATA *pHd,const BYTE * in,BYTE *out);

#endif
