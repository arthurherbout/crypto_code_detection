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

#ifndef ANUBIS_DATA_H
#define ANUBIS_DATA_H

// ** Thread-safe implementation

// ** Anubis cipher
// ** 128bit block size
// ** 256bit key

#define MIN_N			 4 
#define MAX_N			10 
#define MIN_ROUNDS		(8 + MIN_N) 
#define MAX_ROUNDS		(8 + MAX_N) 
#define MIN_KEYSIZEB	(4*MIN_N) 
#define MAX_KEYSIZEB	(4*MAX_N) 
#define BLOCKSIZE		128 
#define BLOCKSIZEB		(BLOCKSIZE/8) 

/* 
 * The KEYSIZEB macro should be redefined for each allowed key size 
 * in order to use the NESSIE test vector generator program. 
 * Valid sizes (in bytes) are 16, 20, 24, 28, 32, 36, and 40. 
 */ 
#define KEYSIZEB		32
 
typedef struct { 
	int keyBits; /* this field must be initialized before the NESSIEkeysetup call */ 
	int R; 
	DWORD roundKeyEnc[MAX_ROUNDS + 1][4]; 
	DWORD roundKeyDec[MAX_ROUNDS + 1][4]; 
} ANUBIS_DATA;

#endif
