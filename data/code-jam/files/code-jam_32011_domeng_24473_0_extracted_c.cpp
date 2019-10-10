#include <string>
#include <vector>
#include <cstdio>
#include <cmath>
#include <map>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
using namespace std;
int R,C;
inline int BN(int x) { return 1<<x; }
int COUNT(int st)
{
	int q,cnt=0;
	for (q=0;q<C;q++) if (st & BN( (R/2) * C + q ) ) cnt++;
	return cnt;
}
int Data[5][5];
int Result[5][5],Sum[5][5];
int profit(int st)
{
	int q,w,e,r,s;
	for (q=0;q<R;q++) for (w=0;w<C;w++) 
	{
		if (st&BN(q*C+w)) Result[q][w]=1;
		else Result[q][w]=0;
	}	
	for (q=0;q<R;q++) for (w=0;w<C;w++)
	{
		s=0;
		for (e=-1;e<=1;e++) for (r=-1;r<=1;r++) if (q+e>=0 && q+e<R && w+r>=0 && w+r<C)
			s+=Result[q+e][w+r];
		if (s!=Data[q][w]) return 0;
	}
	return 1;
}
int main()
{
	srand((unsigned)time(NULL));
	freopen("C-small-attempt0.in","r",stdin);
	freopen("C-small.out","w",stdout);
	int kase,T;
	scanf("%d",&T);
	for (kase=1;kase<=T;kase++)
	{
		fprintf(stderr,"%d\n",kase);
		scanf("%d %d",&R,&C);
		int q,w,ret=0;
		for (q=0;q<R;q++) for (w=0;w<C;w++) scanf("%d",Data[q]+w);
		for (q=0;q<BN(R*C);q++)
		{
			int t;
			if ((t=COUNT(q))<=ret) continue;
			if (profit(q)) ret=t;
		}
		printf("Case #%d: %d\n",kase,ret);
	}
	return 0;
}