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

#ifndef UNICORN_H
#define UNICORN_H

// ** Thread-safe implementation

// ** Unicorn-A cipher
// ** 128bit block size
// ** 256bit key

extern	void Unicorn_set_key(BYTE *eKey,const BYTE *secret);
extern	void Unicorn_encrypt(const BYTE *eKey,const BYTE *src,BYTE *dst);
extern	void Unicorn_decrypt(const BYTE *eKey,const BYTE *src,BYTE *dst);

#endif
