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

#ifndef IDEANXT128_DATA_H
#define IDEANXT128_DATA_H

// ** Thread-safe implementation

// ** Idea-Nxt cipher
// ** 128bit block size
// ** 256bit key

// min 16 ... max 255
#define NXT128_TOTAL_ROUNDS 16

typedef struct {
    DWORD rk[NXT128_TOTAL_ROUNDS * 4];
} nxt128_ctx;

#endif
