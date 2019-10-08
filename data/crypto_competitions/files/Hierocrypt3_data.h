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

#ifndef HC3_DATA_H
#define HC3_DATA_H

// ** Thread-safe implementation

// ** Hierocrypt3 cipher
// ** 128bit block size
// ** 256bit key

#define MAX_ROUND (8)

typedef BYTE HC3_KS[MAX_ROUND + 1][8][4];

typedef struct {
	HC3_KS ks;
	HC3_KS dks;
} HIEROCRYPT3_DATA;

#endif
