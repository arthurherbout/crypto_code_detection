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

#ifndef CSPRNG_H
#define CSPRNG_H

// ** Thread-safe implementation

// ** Cryptographically Secure Pseudo Random Number Generator (by construction)
// ** Based on AES-256 + CTR

// "hashE" = hashing function
// "passw" = password, byte[MAX_PASSW_SIZE]
// "nonce" = cryptographic nonce (number used once)

#include "CSPRNG_data.h"

extern	void CSPRNG_set_seed(CSPRNG_DATA *pCd,ENUM_HASH hashE,const BYTE *passw,DWORD nonce);
extern	void CSPRNG_autoseed(CSPRNG_DATA *pCd,perc_callback_t backFunc,void *desc);

extern	BYTE CSPRNG_get_byte(CSPRNG_DATA *pCd);
extern	WORD CSPRNG_get_word(CSPRNG_DATA *pCd);
extern	DWORD CSPRNG_get_dword(CSPRNG_DATA *pCd);
extern	OBFUNC_RETV CSPRNG_randomize(CSPRNG_DATA *pCd,const DWORD len,BYTE *buf,perc_callback_t pFunc,void *pDesc,test_callback_t tFunc,void *tDesc);
extern	void CSPRNG_array_init(CSPRNG_DATA *pCd,DWORD max,BYTE *buf);

#endif
