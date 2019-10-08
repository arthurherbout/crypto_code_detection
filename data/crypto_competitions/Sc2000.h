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

#ifndef SC2000_H
#define SC2000_H

// ** Thread-safe implementation

// ** Sc2000 cipher
// ** 128bit block size
// ** 256bit key

extern	void Sc2000_set_key(DWORD *ek,const DWORD *in_key);
extern	void Sc2000_encrypt(const DWORD *ek,const DWORD *in, DWORD *out);
extern	void Sc2000_decrypt(const DWORD *ek,const DWORD *in, DWORD *out);

#endif
