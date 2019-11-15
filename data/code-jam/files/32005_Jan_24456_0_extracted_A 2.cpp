/*
	Author       :	Jan
	Problem Name :
	Algorithm    :
	Complexity   :
*/

#include <vector>
#include <list>
#include <map>
#include <set>
#include <queue>
#include <stack>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <cctype>
#include <string>

using namespace std;

#define min(a,b) ((a) < (b) ? (a) : (b))
#define max(a,b) ((a) > (b) ? (a) : (b))

#define CLR(a) memset(a,0,sizeof(a))

#define i64 long long
#define u64 unsigned i64

#define MAX 100001

int cases,caseno,n,m,sv[10001][2],K,hmin,hmax,wmin,wmax,hcmax,hcmin,wcmax,wcmin;

void input()
{
	int i,H,W;
	char a[1000],b[109];
	scanf("%d",&n);

	K=0;

	gets(a);
	hmin=1000000000;
	hmax=-1000000000;

	wmin=1000000000;
	wmax=-1000000000;

	hcmax=1000000000;
	hcmin=-1000000000;

	wcmin=-1000000000;
	wcmax=1000000000;
	for(i=0;i<n;i++)
	{
		gets(a);
		sscanf(a,"%d %d %s",&H,&W,b);
		if(b[0]=='B')
		{
			hmax=max(hmax,H);
			hmin=min(hmin,H);

			wmax=max(wmax,W);
			wmin=min(wmin,W);
		}
		else
		{
			sv[K][0]=H;
			sv[K][0]=W;

			hcmin=max(hcmin,H);
			hcmax=min(hcmax,H);

			wcmin=max(wcmin,W);
			wcmax=min(wcmax,W);
		}
	}
	//printf("%d %d %d %d\n",hcmin,hcmax,wcmin,wcmax);
}

void process()
{
	int i,H,W;

	scanf("%d",&m);
	printf("Case #%d:\n",++caseno);
	for(i=0;i<m;i++)
	{
		scanf("%d %d",&H,&W);
		if(H>=hmin && H<=hmax && W>=wmin && W<=wmax) puts("BIRD");
		else if(H>=hmin && H<=hmax)
		{
			if(W>=wmax && W<wcmax) puts("UNKNOWN");
			else if(W<=wmin && W>wcmin) puts("UNKNOWN");
			else puts("NOT BIRD");
		}
		else if(W>=wmin && W<=wmax)
		{
			if(H>=hmax && H<hcmax) puts("UNKNOWN");
			else if(H<=hmin && H>hcmin) puts("UNKNOWN");
			else puts("NOT BIRD");
		}
		//else if(H>=hcmax || H<=hcmin || W>=wcmax || W<=wcmin) puts("NOT BIRD");
		else puts("UNKNOWN");
	}
}

int main()
{
	freopen("a.in","r",stdin);
	freopen("a.ans","w",stdout);

	scanf("%d",&cases);
	while(cases--)
	{
		input();
		process();
	}
	return 0;
}
