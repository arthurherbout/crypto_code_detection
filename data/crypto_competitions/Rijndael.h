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

#ifndef RIJNDAEL_H
#define RIJNDAEL_H

// ** Thread-safe implementation

// ** Aes cipher
// ** 128bit block size
// ** 256bit key

#include "Rijndael_data.h"

extern	int Rijndael_set_key_encrypt(DWORD *rk,const BYTE *key,const int keybits);
extern	int Rijndael_set_key_decrypt(DWORD *rk,const BYTE *key,const int keybits);
extern	void Rijndael_encrypt(const DWORD *rk,const int nrounds,const BYTE *plaintext,BYTE *ciphertext);
extern	void Rijndael_decrypt(const DWORD *rk,const int nrounds,const BYTE *ciphertext,BYTE *plaintext);

#endif
