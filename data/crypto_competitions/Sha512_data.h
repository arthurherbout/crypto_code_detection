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

#ifndef SHA512_DATA_H
#define SHA512_DATA_H

// ** Thread-safe implementation

// ** Sha2 hashing
// ** 512bit hash

typedef unsigned __int64	QWORD;

typedef struct {
	DWORD	inputLen;
	QWORD	A,B,C,D,E,F,G,H;
	QWORD	totalLen;
	BYTE	input[128];
} SHA512_DATA;

#endif
