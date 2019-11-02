#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>
#include <map>
#include <queue>
#include <iostream>
using namespace std;
int n,m;
#define  LL  long long
#define maxn 1010100
LL a[maxn],sum[maxn];
int main()
{
	int i,j,p,q,r,s,ncase,tt=0;
	freopen("A-small-attempt1.in","r",stdin);
	freopen("A-small-attempt1.out","w",stdout);
	scanf("%d",&ncase);
	while(ncase--)
	{
		scanf("%d %d %d %d %d",&n,&p,&q,&r,&s);
		for(i=1;i<=n;i++)
		{
			long long tmp=p; tmp=tmp*(i-1);
			a[i]=(tmp+q)%r+s;
			sum[i]=sum[i-1]+a[i];
		}
		LL ans=0;
		for(i=1;i<=n;i++)
		{
			for(j=i;j<=n;j++)
			{
				LL aa=sum[i-1];
				LL bb=sum[j]-sum[i-1];
				LL cc=sum[n]-sum[j];
				ans=max(ans,sum[n]-max(aa,max(bb,cc)));
			}
		}
		printf("Case #%d: %.9lf\n",++tt,ans*1.0/sum[n]);
	}
	return 0;
}