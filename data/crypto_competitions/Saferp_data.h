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

#ifndef SAFERP_DATA_H
#define SAFERP_DATA_H

// ** Thread-safe implementation

// ** Safer+ cipher
// ** 128bit block size
// ** 256bit key

typedef struct {
	BYTE	l_key[33 * 16];
	DWORD	k_bytes;
} SAFERP_DATA;

#endif
