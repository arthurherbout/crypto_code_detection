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

#ifndef FROG_DATA_H
#define FROG_DATA_H

// ** Thread-safe implementation

// ** Frog cipher
// ** 128bit block size
// ** 256bit key

typedef struct  
{   BYTE  k_xbu[16];
    BYTE  k_spu[256];
    BYTE  k_bpu[16];
} k_str;

typedef struct
{   k_str   f_key[8];
    BYTE  i_key[8][256];
} key_str;

typedef struct {
	key_str loc_key;
	key_str sim_key;
	key_str	*lkp;
} FROG_DATA;

#endif
