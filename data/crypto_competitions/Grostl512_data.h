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

#ifndef GROSTL512_DATA_H
#define GROSTL512_DATA_H

// ** Thread-safe implementation

// ** Grostl hashing
// ** 512bit hash

typedef unsigned __int64	QWORD;

#define ROWS 8
#define LENGTHFIELDLEN ROWS
#define COLS1024 16
#define SIZE1024 (ROWS*COLS1024)
#define ROUNDS1024 14

typedef struct {
  BYTE chaining[ROWS][COLS1024]; /* the actual state */
  QWORD block_counter; /* block counter */
  int hashbitlen; /* output length */
  BYTE buffer[SIZE1024]; /* block buffer */
  int buf_ptr; /* buffer pointer */
  int bits_in_last_byte; /* number of bits in incomplete byte */
  int columns; /* number of columns in state */
  int rounds; /* number of rounds in P and Q */
  int statesize; /* size of state (ROWS*columns) */
} GROSTL512_DATA;

#endif
