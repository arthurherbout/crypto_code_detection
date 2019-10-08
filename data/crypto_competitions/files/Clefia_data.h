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

#ifndef CLEFIA_DATA_H
#define CLEFIA_DATA_H

// ** Thread-safe implementation

// ** Clefia cipher
// ** 128bit block size
// ** 256bit key

typedef struct {
	int		r;
	BYTE	rk[8 * 26 + 16];
} CLEFIA_DATA;

#endif
