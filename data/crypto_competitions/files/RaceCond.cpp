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

#include "CommonDef.h"

#define	CYCLE_NUM	100000
#define	THREAD_NUM	4

BYTE			sharedValue=0;
volatile BOOL	sharedOkToGo;
volatile BOOL	sharedStop;

DWORD	concThread_param[THREAD_NUM];
HANDLE	concThread_semaphores1[THREAD_NUM];
HANDLE	concThread_semaphores2[THREAD_NUM];

void concThread(void *param)
{
	DWORD	threadN=*((DWORD *) param);

	while(TRUE)
		{
		DWORD			index;

		// 1. synch
		WaitForSingleObject(concThread_semaphores1[threadN],INFINITE);
		ReleaseSemaphore(concThread_semaphores2[threadN],1,NULL);

		if(sharedStop)
			{ _endthread(); }

		// start "sharedValue" concurrent modify
		while(!sharedOkToGo);

		for(index=0;index<(CYCLE_NUM/128);index++)
			{
			LARGE_INTEGER	li;

			QueryPerformanceCounter(&li);

			if(threadN%2)
				{
				__asm
					{
					movzx ecx,byte ptr [li]
					jecxz LOOP0_PREHEAD
					jmp LOOP0_HEAD

					LOOP0_PREHEAD:
					inc ecx
					LOOP0_HEAD:
					dec sharedValue
					loop LOOP0_HEAD
					}
				}
			else
				{
				__asm
					{
					movzx ecx,byte ptr [li]
					jecxz LOOP1_PREHEAD
					jmp LOOP1_HEAD

					LOOP1_PREHEAD:
					inc ecx
					LOOP1_HEAD:
					inc sharedValue
					loop LOOP1_HEAD
					}
				}
			}

		ReleaseSemaphore(concThread_semaphores2[threadN],1,NULL);
		}
}

void GenerateEntropy(const DWORD len,BYTE *outBuf,perc_callback_t backFunc,void *desc)
{
	DWORD	tLen=len;
	DWORD	index;

	sharedOkToGo=FALSE;
	sharedStop=FALSE;

	memset(outBuf,0,len);
	memset(concThread_semaphores1,0,THREAD_NUM*sizeof(HANDLE));
	memset(concThread_semaphores2,0,THREAD_NUM*sizeof(HANDLE));

	// N semaphores : starting count=0
	for(index=0;index<THREAD_NUM;index++)
		{
		if(	(!(concThread_semaphores1[index]=CreateSemaphore(NULL,0,1,NULL)))||
			(!(concThread_semaphores2[index]=CreateSemaphore(NULL,0,1,NULL))) )
			{
			for(index=0;index<THREAD_NUM;index++)
				{
				if(concThread_semaphores1[index])
					{ CloseHandle(concThread_semaphores1[index]); }
				if(concThread_semaphores2[index])
					{ CloseHandle(concThread_semaphores2[index]); }
				}
		
			return;
			}
		}

	// problem: thread creation time >> CPU bound time
	// solution: create and leave it running until buf end
	for(index=0;index<THREAD_NUM;index++)
		{
		concThread_param[index]=index;

		_beginthread(concThread,NULL,(void *) &concThread_param[index]);
		}

	while(tLen)
		{
		DWORD	index;

		sharedOkToGo=FALSE;
		
		for(index=0;index<THREAD_NUM;index++)
			{ ReleaseSemaphore(concThread_semaphores1[index],1,NULL); }
		WaitForMultipleObjects(THREAD_NUM,concThread_semaphores2,TRUE,INFINITE);

		// start "sharedValue" concurrent modify
		sharedOkToGo=TRUE;

		WaitForMultipleObjects(THREAD_NUM,concThread_semaphores2,TRUE,INFINITE);

		if(backFunc)
			{ backFunc(desc,(BYTE) ((((float) (len-tLen))/((float) len))*((float) 100))); }

		*outBuf=sharedValue;
		outBuf++;
		tLen--;
		}

	sharedStop=TRUE;

	for(index=0;index<THREAD_NUM;index++)
		{ ReleaseSemaphore(concThread_semaphores1[index],1,NULL); }
	WaitForMultipleObjects(THREAD_NUM,concThread_semaphores2,TRUE,INFINITE);

	for(index=0;index<THREAD_NUM;index++)
		{
		CloseHandle(concThread_semaphores1[index]);
		CloseHandle(concThread_semaphores2[index]);
		}
}
