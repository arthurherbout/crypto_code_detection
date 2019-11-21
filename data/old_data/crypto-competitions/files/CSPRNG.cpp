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

#include "stdafx.h"

#include "RaceCond.h"
#include "Sha512.h"
#include "Grostl512.h"
#include "Keccak512.h"
#include "Skein512.h"
#include "MultiBase.h"

#include "CSPRNG_data.h"

#define	RESEED_COUNTDOWN	1000

void CSPRNG_set_seed(CSPRNG_DATA *pCd,ENUM_HASH hashE,const BYTE *passw,DWORD nonce)
{
	BYTE	inBuf[64];
	BYTE	hash[64];
	DWORD	len;

	memset(pCd,0,sizeof(CSPRNG_DATA));

	memset(inBuf,0,64);
	memcpy(inBuf,passw,MAX_PASSW_SIZE);
	len=strlen((char *) inBuf);
	memcpy(&inBuf[len],&nonce,4);
	len+=4;

	if(hashE==SHA512_HASH)
		{
		SHA512_DATA		sha;

		Sha512_init(&sha);
		Sha512_data(&sha,inBuf,len);
		Sha512_finalize(&sha,hash);
		}
	else if(hashE==GROSTL512_HASH)
		{
		GROSTL512_DATA	grostl;

		Grostl512_init(&grostl);
		Grostl512_data(&grostl,inBuf,len);
		Grostl512_finalize(&grostl,hash);
		}
	else if(hashE==KECCAK512_HASH)
		{
		KECCAK512_DATA	keccak;

		Keccak512_init(&keccak);
		Keccak512_data(&keccak,inBuf,len);
		Keccak512_finalize(&keccak,hash);
		}
	else if(hashE==SKEIN512_HASH)
		{
		SKEIN512_DATA	skein;

		Skein512_init(&skein);
		Skein512_data(&skein,inBuf,len);
		Skein512_finalize(&skein,hash);
		}

	Multi_single_setkey(&pCd->msd,RIJNDAEL_ALG,hash);

	memcpy(pCd->ctrBuf,&hash[MAX_PASSW_SIZE],DATA_BLOCK_SIZE);
	pCd->availCount=0;	
}

// auto setup with default
void CSPRNG_autoseed(CSPRNG_DATA *pCd,perc_callback_t backFunc,void *desc)
{
	BYTE	passw[MAX_PASSW_SIZE+DATA_BLOCK_SIZE];
	BYTE	nullPassw[MAX_PASSW_SIZE+DATA_BLOCK_SIZE];

	memset(pCd,0,sizeof(CSPRNG_DATA));

	memset(nullPassw,0,MAX_PASSW_SIZE+DATA_BLOCK_SIZE);
	GenerateEntropy(MAX_PASSW_SIZE+DATA_BLOCK_SIZE,passw,backFunc,desc);

	// GenerateEntropy KO
	if(!memcmp(passw,nullPassw,MAX_PASSW_SIZE+DATA_BLOCK_SIZE))
		{
		DWORD	index;

		LARGE_INTEGER	qpc;

		QueryPerformanceCounter(&qpc);

		srand(*((DWORD *) &qpc));

		for(index=0;index<(MAX_PASSW_SIZE+DATA_BLOCK_SIZE);index++)
			{ passw[index]=(BYTE) rand(); }
		}

	Multi_single_setkey(&pCd->msd,RIJNDAEL_ALG,passw);

	memcpy(pCd->ctrBuf,&passw[MAX_PASSW_SIZE],DATA_BLOCK_SIZE);
	pCd->availCount=0;	
}

// little endian
void BlockInc(BYTE *data)
{
	#if	DATA_BLOCK_SIZE==16
	__asm
		{
		mov edi,[data]
		clc

		inc dword ptr [edi+ 0]
		adc dword ptr [edi+ 4],0
		adc dword ptr [edi+ 8],0
		adc dword ptr [edi+12],0
		}
	#else
	#error
	#endif
}

// auto reseeding leads to a bad chi-square result after some Mb of data!
BYTE CSPRNG_get_byte(CSPRNG_DATA *pCd)
{
	if(!pCd->availCount)
		{
		// random = AES-256(CTR)
		Multi_ECB_single_encrypt(&pCd->msd,RIJNDAEL_ALG,pCd->ctrBuf,pCd->randBuf);
		BlockInc(pCd->ctrBuf);

		pCd->availCount=DATA_BLOCK_SIZE-1;

		return(*pCd->randBuf);
		}
	else
		{ return(pCd->randBuf[DATA_BLOCK_SIZE-(pCd->availCount--)]); }
}

WORD CSPRNG_get_word(CSPRNG_DATA *pCd)
{
	WORD	retV;

	retV=(WORD) CSPRNG_get_byte(pCd);
	retV<<=8;
	retV|=(WORD) CSPRNG_get_byte(pCd);

	return(retV);
}

DWORD CSPRNG_get_dword(CSPRNG_DATA *pCd)
{
	DWORD	retV;

	retV=(DWORD) CSPRNG_get_word(pCd);
	retV<<=16;
	retV|=(DWORD) CSPRNG_get_word(pCd);

	return(retV);
}

#define	REFRESH_COUNTDOWN	1000

OBFUNC_RETV CSPRNG_randomize(CSPRNG_DATA *pCd,const DWORD len,BYTE *buf,perc_callback_t pFunc,void *pDesc,test_callback_t tFunc,void *tDesc)
{
	DWORD	tLen=len;
	BYTE	lastPerc=0;
	WORD	refCount=REFRESH_COUNTDOWN;

	while(tLen--)
		{
		*(buf++)=CSPRNG_get_byte(pCd);

		if(!refCount)
			{
			refCount=REFRESH_COUNTDOWN;

			if(pFunc)
				{
				BYTE	tmp=(BYTE) ((((float) (len-tLen))/((float) len))*((float) 100));
				if(tmp>lastPerc)
					{
					lastPerc=tmp;
					pFunc(pDesc,lastPerc);
					}
				}
			if(tFunc&&tFunc(tDesc))
				{ return(OBFUNC_STOP); }
			}

		refCount--;
		}

	return(OBFUNC_OK);
}

void CSPRNG_array_init(CSPRNG_DATA *pCd,DWORD max,BYTE *buf)
{
	DWORD	index;

	memset(buf,0xFF,max);
	for(index=0;index<max;index++)
		{
		DWORD	rIndex;

		do
			{ rIndex=CSPRNG_get_byte(pCd)%max; }
		while(buf[rIndex]!=0xFF);

		buf[rIndex]=(BYTE) index;
		}
}
