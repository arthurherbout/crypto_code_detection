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

#ifndef SKEIN512_DATA_H
#define SKEIN512_DATA_H

// ** Thread-safe implementation

// ** Skein hashing
// ** 512bit hash

typedef unsigned __int64	QWORD;

#define  SKEIN_MODIFIER_WORDS  ( 2)

#define  SKEIN_512_STATE_WORDS ( 8)
#define  SKEIN_512_STATE_BYTES ( 8*SKEIN_512_STATE_WORDS)
#define  SKEIN_512_STATE_BITS  (64*SKEIN_512_STATE_WORDS)
#define  SKEIN_512_BLOCK_BYTES ( 8*SKEIN_512_STATE_WORDS)

typedef struct {
	DWORD  hashBitLen;				/* size of hash result, in bits */
	DWORD  bCnt;					/* current byte count in buffer b[] */
	QWORD  T[SKEIN_MODIFIER_WORDS];	/* tweak words: T[0]=byte cnt, T[1]=flags */
} Skein_Ctxt_Hdr_t;

typedef struct {
	Skein_Ctxt_Hdr_t h;							/* common header context variables */
	QWORD			X[SKEIN_512_STATE_WORDS];	/* chaining variables */
	BYTE			b[SKEIN_512_BLOCK_BYTES];	/* partial block buffer (8-byte aligned) */
} SKEIN512_DATA;

#endif
