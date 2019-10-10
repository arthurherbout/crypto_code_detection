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

#ifndef CAMELLIA_H
#define CAMELLIA_H

// ** Thread-safe implementation

// ** Camellia cipher
// ** 128bit block size
// ** 256bit key

extern	void Camellia_set_key(BYTE *e,const int n,const BYTE *k);
extern	void Camellia_encrypt(const BYTE *e,const int n,const BYTE *p,BYTE *c);
extern	void Camellia_decrypt(const BYTE *e,const int n,const BYTE *c,BYTE *p);

#endif
