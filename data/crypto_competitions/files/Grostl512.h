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

#ifndef GROSTL512_H
#define GROSTL512_H

// ** Thread-safe implementation

// ** Grostl hashing
// ** 512bit hash

#include "Grostl512_data.h"

extern	void Grostl512_init(GROSTL512_DATA *grostl);
extern	void Grostl512_data(GROSTL512_DATA *grostl,const BYTE *buffer,DWORD len);
extern	void Grostl512_finalize(GROSTL512_DATA *grostl,BYTE *hash);

#endif
