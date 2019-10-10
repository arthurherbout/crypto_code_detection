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

#ifndef TWOFISH_DATA_H
#define TWOFISH_DATA_H

// ** Thread-safe implementation

// ** Twofish cipher
// ** 128bit block size
// ** 256bit key

#define Q_TABLES
#define M_TABLE
#define MK_TABLE
#define ONE_STEP

typedef struct {
	DWORD	k_len;
	DWORD	l_key[40];
	DWORD	s_key[4];

	#ifdef  Q_TABLES
	DWORD	qt_gen;
	BYTE	q_tab[2][256];
	#endif

	#ifdef  M_TABLE
	DWORD	mt_gen;
	DWORD	m_tab[4][256];
	#endif

	#ifdef  MK_TABLE
	#ifdef  ONE_STEP
	DWORD	mk_tab[4][256];
	#else
	BYTE	sb[4][256];
	#endif
	#endif
} TWOFISH_DATA;

#endif
