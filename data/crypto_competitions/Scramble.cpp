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

#include "stdafx.h"

#include "CSPRNG.h"
#include "Scramble_data.h"

void Scramble_build_list(CSPRNG_DATA *pCd,const DWORD count,const DWORD max,DWORD *list)
{
	DWORD	tCount=count;

	while(tCount)
		{
		do
			{
			list[0]=CSPRNG_get_dword(pCd)%max;
			list[1]=CSPRNG_get_dword(pCd)%max;
			}
		while(list[0]==list[1]);

		list+=2;
		tCount--;
		}
}

OBFUNC_RETV Scramble_seed(SCRAMBLE_DATA *pSd,const DWORD len,const BYTE *passw,DWORD nonce)
{
	memset(pSd,0,sizeof(SCRAMBLE_DATA));

	// CSPRNG <- Skein512(passw + nonce)
	CSPRNG_set_seed(&pSd->cd,SKEIN512_HASH,passw,nonce);

	pSd->len=len;

	// 50% scramble : (len/2)*2*sizeof(DWORD)
	if(pSd->list=(DWORD *) malloc(((len>>1)<<1)*sizeof(DWORD)))
		{
		Scramble_build_list(&pSd->cd,len>>1,len,pSd->list);
		return(OBFUNC_OK);
		}
	else
		{ return(OBFUNC_OUTOFMEM); }
}

void Scramble_end(SCRAMBLE_DATA *pSd)
{
	TEST_AND_FREE(pSd->list)
}

#define	REFRESH_COUNTDOWN	10000

OBFUNC_RETV Seg_scramble(SCRAMBLE_DATA *pSd,BYTE *buf,perc_callback_t pFunc,void *pDesc,test_callback_t tFunc,void *tDesc)
{
	DWORD	tot=pSd->len>>1;	// 50%
	DWORD	*pL=pSd->list;
	DWORD	index;		
	BYTE	lastPerc=0;
	WORD	refCount=REFRESH_COUNTDOWN;

	for(index=0;index<tot;index++)
		{
		DWORD	idx0=*(pL++);
		DWORD	idx1=*(pL++);
		BYTE	tmp;

		tmp=buf[idx0];
		buf[idx0]=buf[idx1];
		buf[idx1]=tmp;

		if(!refCount)
			{
			refCount=REFRESH_COUNTDOWN;

			if(pFunc)
				{
				tmp=(BYTE) ((((float) index)/((float) tot))*((float) 100));
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

OBFUNC_RETV Seg_descramble(SCRAMBLE_DATA *pSd,BYTE *buf,perc_callback_t pFunc,void *pDesc,test_callback_t tFunc,void *tDesc)
{
	DWORD	tot=pSd->len>>1;	// 50%
	DWORD	*pL=&pSd->list[((pSd->len>>1)<<1)-1];
	DWORD	index;		
	BYTE	lastPerc=0;
	WORD	refCount=REFRESH_COUNTDOWN;

	for(index=0;index<tot;index++)
		{
		DWORD	idx1=*(pL--);
		DWORD	idx0=*(pL--);
		BYTE	tmp;

		tmp=buf[idx0];
		buf[idx0]=buf[idx1];
		buf[idx1]=tmp;

		if(!refCount)
			{
			refCount=REFRESH_COUNTDOWN;

			if(pFunc)
				{
				tmp=(BYTE) ((((float) index)/((float) tot))*((float) 100));
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
