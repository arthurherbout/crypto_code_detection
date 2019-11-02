#include <string>
#include <vector>
#include <cstdio>
#include <cmath>
#include <map>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
using namespace std;
int Req[5000][3];
int BN(int x)
{
	return 1<<x;
}
int cnt(int x)
{
	int c=0;
	while (x)
	{
		if (x&1) c++;
		x>>=1;
	}
	return c;
}
int main()
{
	srand((unsigned)time(NULL));
	freopen("A-small-attempt0.in","r",stdin);
	freopen("A-small.out","w",stdout);
	int kase,T,N;
	scanf("%d",&T);
	//T=0;//rand();
	for (kase=1;kase<=T;kase++)
	{
		scanf("%d",&N);
		int q,w,miv[3],ret=0;
		for (q=0;q<N;q++) for (w=0;w<3;w++) scanf("%d",Req[q]+w);
		for (q=0;q<BN(N);q++)
		{
			int s=0,e;
			for (w=0;w<3;w++) miv[w]=0;
			for (w=0;w<N;w++) 
				if (q & BN(w))
					for (e=0;e<3;e++) miv[e]=max(miv[e],Req[w][e]);
			for (w=0;w<3;w++) s+=miv[w];
			if (s<=10000) ret=max(ret,cnt(q));
		}
		printf("Case #%d: %d\n",kase,ret);
	}
	return 0;
}