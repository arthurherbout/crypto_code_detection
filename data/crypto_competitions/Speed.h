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

#ifndef SPEED_H
#define SPEED_H

// ** Thread-safe implementation

// ** Speed cipher
// ** 128bit block size
// ** 256bit key

#include "Speed_data.h"

extern	void Speed_set_key(speed_ikey rndkey,const speed_key key);
extern	void Speed_encrypt(const speed_ikey rndkey,const speed_data pt,speed_data ct);
extern	void Speed_decrypt(const speed_ikey rndkey,const speed_data ct,speed_data pt);

#endif
