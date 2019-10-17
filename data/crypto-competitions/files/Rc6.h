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

#ifndef RC6_H
#define RC6_H

// ** Thread-safe implementation

// ** Rc6 cipher
// ** 128bit block size
// ** 256bit key

extern	void Rc6_set_key(DWORD *l_key,const DWORD *in_key, const DWORD key_len);
extern	void Rc6_encrypt(const DWORD *l_key,const DWORD *in_blk, DWORD *out_blk);
extern	void Rc6_decrypt(const DWORD *l_key,const DWORD *in_blk, DWORD *out_blk);

#endif
