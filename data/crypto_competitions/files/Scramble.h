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

#ifndef SCRAMBLE_H
#define SCRAMBLE_H

// ** Thread-safe implementation

// ** Scramble data segment
// ** Based on CSPRNG

// "passw" = password, byte[MAX_PASSW_SIZE]
// "nonce" = cryptographic nonce (number used once)

#include "Scramble_data.h"

extern	OBFUNC_RETV Scramble_seed(SCRAMBLE_DATA *pSd,const DWORD len,const BYTE *passw,DWORD nonce);
extern	void Scramble_end(SCRAMBLE_DATA *pSd);

extern	OBFUNC_RETV Seg_scramble(SCRAMBLE_DATA *pSd,BYTE *buf,perc_callback_t pFunc,void *pDesc,test_callback_t tFunc,void *tDesc);
extern	OBFUNC_RETV Seg_descramble(SCRAMBLE_DATA *pSd,BYTE *buf,perc_callback_t pFunc,void *pDesc,test_callback_t tFunc,void *tDesc);

#endif
