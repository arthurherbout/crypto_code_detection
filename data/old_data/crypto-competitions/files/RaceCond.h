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

#ifndef RACECOND_H
#define RACECOND_H

// ** Generate some entropy
// ** Use multithread RaceCondition

#include "CommonDef.h"

extern	void GenerateEntropy(const DWORD len,BYTE *outBuf,perc_callback_t backFunc,void *desc);

#endif
