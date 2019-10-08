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

#ifndef ANUBIS_H
#define ANUBIS_H

// ** Thread-safe implementation

// ** Anubis cipher
// ** 128bit block size
// ** 256bit key

#include "Anubis_data.h"

extern	void Anubis_set_key(ANUBIS_DATA *pAd,const BYTE *key);
extern	void Anubis_encrypt(const ANUBIS_DATA *pAd,const BYTE *plaintext,BYTE *ciphertext);
extern	void Anubis_decrypt(const ANUBIS_DATA *pAd,const BYTE *ciphertext,BYTE *plaintext);

#endif
