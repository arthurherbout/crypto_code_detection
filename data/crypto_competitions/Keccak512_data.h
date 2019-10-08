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

#ifndef KECCAK512_DATA_H
#define KECCAK512_DATA_H

// ** Thread-safe implementation

// ** Keccak hashing
// ** 512bit hash

typedef unsigned __int64	QWORD;

#define cKeccakB    1600
#define cKeccakR    512

typedef struct {
	BYTE	state[cKeccakB / 8];
	int		bitsInQueue;
} KECCAK512_DATA;

#endif
