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

#ifndef MULTI_H
#define	MULTI_H

// ** Thread-safe implementation

// ** Wrapping of implemented ciphers
// ** 128bit block size	(DATA_BLOCK_SIZE<<3)
// ** 2x 256bit keys	(MAX_PASSW_SIZE<<3)
// ** Cypher-Block-Chaining (CBC)

// "iv" = Inizialization Vectors (IVs), byte[MAX_ALG][DATA_BLOCK_SIZE]
// "passw1" = password1, byte[MAX_PASSW_SIZE]
// "passw2" = password2, byte[MAX_PASSW_SIZE]
// "nonce" = cryptographic nonce (number used once)

#include "Multi_data.h"

extern	void Multi_setkey(MULTI_DATA *pMd,const BYTE *iv,const BYTE *passw1,const BYTE *passw2,DWORD nonce);

extern	OBFUNC_RETV Multi_CBC_encrypt(MULTI_DATA *pMd,const DWORD len,BYTE *buf,perc_callback_t pFunc,void *pDesc,test_callback_t tFunc,void *tDesc);
extern	OBFUNC_RETV Multi_CBC_decrypt(MULTI_DATA *pMd,const DWORD len,BYTE *buf,perc_callback_t pFunc,void *pDesc,test_callback_t tFunc,void *tDesc);

#endif
