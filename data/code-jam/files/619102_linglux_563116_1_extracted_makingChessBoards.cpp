#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#pragma warning(disable:4786)
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

typedef __int64 int64;
typedef unsigned __int64 u_int64;
typedef unsigned int     u_int;
typedef u_int64  Type;
typedef unsigned char u_char;

char pboard[512][512];
map<u_int,u_int> maps;

void cut(int M,int N);

int main(int argc,char* argv[])
{
	if( argc <= 1)
		return printf("need file name as a  parameter!"); 
	FILE* pf = fopen(argv[1],"r");
	if( pf == NULL)
		return printf("cann't open file");
	
	u_int T = 0;
	fscanf(pf,"%d\n",&T);
	
	int i=0,j=0,r=0,k=0;
	for(i=0;i<T;i++)
	{
		maps.clear();

		u_int M,N;
		fscanf(pf,"%d %d\n",&M,&N);

		char buf[512];
		char ch=0;
		
		maps.clear();
		for(j=0;j<M;j++)
		{
			fscanf(pf,"%s\n",buf);
			for(r=0;r<(N/4);r++)
			{
				ch = buf[r];
				if( (ch >= '0') && (ch <= '9') )
					ch = ch - '0';
				else
					ch = ch - 'A' + 10;

				pboard[j][4*r+0] = (ch & 8)>>3;
				pboard[j][4*r+1] = (ch & 4)>>2;
				pboard[j][4*r+2] = (ch & 2)>>1;
				pboard[j][4*r+3] = (ch & 1)>>0;
			}
		}

		/*
		for(j=0;j<M;j++)
		{
			for(r=0;r<N;r++)
				printf("%d ",pboard[j][r]);
			printf("\n");
		}
		*/
		
		cut(M,N);

		printf("Case #%d: %u\n",i+1,maps.size());
		vector<u_int> its;
		
		map<u_int,u_int>::iterator it;
		for(it=maps.begin();it!=maps.end();it++)
		{
			u_int key = it->first;
			its.push_back(key);
		}
		sort(its.begin(),its.end());
		for(r=its.size()-1;r>=0;r--)
		{
			u_int key = its[r];
			u_int v   = maps.find(key)->second;
			printf("%u %u\n",key,v);
		}
	}
	
	fclose(pf);
	return 0;
}

int fcheck(int x,int y,int s);
void docheck(int x,int y,int s);

void cut(int M,int N)
{
	int min   = (M<N)?M:N;
	int check = min;

	for(check = min;check>=1;check--)
		for(int i=0;i<=(M-check);i++)
			for(int j=0;j<=(N-check);j++)
			{
				if( pboard[i][j] != 2)
				{
					if( fcheck(i,j,check) ==0 )
						docheck(i,j,check);
				}
			}
}

int fcheck(int x,int y,int s)
{
	if( s == 1 )
	{
		if( pboard[x][y] == 2)
			return -1;
		else
			return 0;
	}

	for(int i=x;i<(x+s);i++)
		for(int j=y;j<(y+s);j++)
		{
			if( pboard[i][j] == 2)
				return -1;
			if( (j>y) && (pboard[i][j] == pboard[i][j-1]) )
				return -1;
			if( (i>x) && (pboard[i-1][j] == pboard[i][j]) )
				return -1;
		}
	return 0;
}

void docheck(int x,int y,int s)
{
	for(int i=x;i<(x+s);i++)
		for(int j=y;j<(y+s);j++)
			pboard[i][j] = 2;

	map<u_int,u_int>::iterator it;
	it = maps.find((u_int)s);
	if( it == maps.end() )
		maps.insert(make_pair((u_int)s,1));
	else
		it->second++;
}