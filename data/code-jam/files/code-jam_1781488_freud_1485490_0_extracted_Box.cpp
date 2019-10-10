// Diamond.cpp : Defines the entry polong long for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <conio.h>
#include <string.h>
FILE *fin,*fout;

long long T,N,M,i,j,k,t;

struct msss {
	long long index;
	long long used;
} topBox,topToy;

struct mys {
	long long value;
	long long type;
} box[111],toy[111];

long long MIN(long long a,long long b) {
	if (a<b)
		return a;
	else
		return b;
}


long long process(long long topBoxIndex,long long topBoxUsed,long long topToyIndex, long long topToyUsed) {
	long long i,j,k;
	long long bestThrowBox,bestThrowToy;

	// end case
	if (topBoxIndex >N || topToyIndex>M)
		return 0;

	// goodcase
	if ( box[topBoxIndex].type == toy[topToyIndex].type) {
		
		long long min,thxGod;
		
		long long vsBox,vsToy;
		vsBox = box[topBoxIndex].value - topBoxUsed;
		vsToy = toy[topToyIndex].value - topToyUsed;

		thxGod = MIN(vsBox,vsToy);

		if (  vsBox > vsToy) {
			// delete some part of box  
			// plus toy + 1
			return thxGod + process(topBoxIndex, thxGod+topBoxUsed,topToyIndex+1,0 );
		}
		else if (vsBox < vsToy) {
			return thxGod + process(topBoxIndex+1,0,topToyIndex, thxGod+topToyUsed);
		} else if (vsBox == vsToy ) {
			return thxGod + process(topBoxIndex+1,0,topToyIndex+1,0);
		}


	}
	// throw some thing
	else {
		bestThrowBox = process(topBoxIndex+1,0,topToyIndex,topToyUsed);
		bestThrowToy = process(topBoxIndex,topBoxUsed,topToyIndex+1,0);
	
		if (bestThrowBox > bestThrowToy)
			return bestThrowBox;
		else
			return bestThrowToy;
	}

	
}

int _tmain(int argc, _TCHAR* argv[])
{
	fin = fopen("Box.txt","r");
	fout=fopen("Box.out","w");


	fscanf(fin,"%lld",&T);
	for (t=1;t<=T;t++) {
		fscanf(fin,"%lld %lld",&N,&M);
		memset(box,0,sizeof(box));
		memset(toy,0,sizeof(toy));
		


		for (i=1;i<=N;i++) {
			fscanf(fin,"%lld %lld",&box[i].value,&box[i].type);
		}
		for (i=1;i<=M;i++) {
			fscanf(fin,"%lld %lld",&toy[i].value,&toy[i].type);
		}

		topBox.index = 1;
		topToy.index = 1;
		topBox.used = 0;
		topToy.used = 0;

		long long answer;
		answer = process(1,0,1,0);

		fprintf(fout,"Case #%lld: %lld\n",t,answer);
	}


 
	fclose(fin);
	fclose(fout);


	return 0;
}

