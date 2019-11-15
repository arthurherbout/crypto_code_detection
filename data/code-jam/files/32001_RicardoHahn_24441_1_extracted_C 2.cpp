#include <stdio.h>
#include <algorithm>
#include <stdlib.h>
#include <math.h>

using namespace std;

#define MAX 1100

typedef struct pto
{
	int x,y,z;
} pto;

pto pos[MAX];

int pi[MAX];

int mdis(const pto & a,const pto & b)
{
	return abs(a.x-b.x)+abs(a.y-b.y)+abs(a.z-b.z);
}

bool chk(double p,int n)
{
	bool ret=1;
	int i,j;
	int d;
	for(i=0;ret && i<n;++i)
		for(j=i+1;ret && j<n;++j)
		{
			d=mdis(pos[i],pos[j]);
			if(d>p*pi[i]+p*pi[j])
				ret=0;
		}
	return ret;
}

double bs(int n)
{
	double b,e,m;
	b=0;e=1e7;
	while(e-b>=1e-6)
	{
		m=b+(e-b)/2;
		if(chk(m,n))
			e=m;
		else
			b=m;
	}
	return m;
}

int main()
{
	int i;
	int t,cnt;
	int n;
	scanf("%d",&t);
	for(cnt=1;cnt<=t;++cnt)
	{
		scanf("%d",&n);
		for(i=0;i<n;++i)
			scanf("%d %d %d %d",&pos[i].x,&pos[i].y,&pos[i].z,&pi[i]);
		printf("Case #%d: %.6f\n",cnt,bs(n));
	}
	return 0;
}





