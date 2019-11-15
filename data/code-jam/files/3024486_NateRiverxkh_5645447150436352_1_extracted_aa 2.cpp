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
	freopen("A-large.in","r",stdin);
	freopen("A-large.out","w",stdout);
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
			LL aa=sum[i-1];
			int ll=i,rr=n+1;
			while(ll<rr)
			{
				int mid=(ll+rr)/2;
				if(sum[n]-sum[mid]>sum[mid]-sum[i-1])
					ll=mid+1;
				else
					rr=mid;
			}
			LL tmp1=max(aa,max(sum[rr]-sum[i-1],sum[n]-sum[rr]));
			LL tmp2=sum[n];
			if(rr+1<=n) tmp2=max(aa,max(sum[rr+1]-sum[i-1],sum[n]-sum[rr+1]));
			LL tmp3=sum[n];
			if(rr-1>=i) tmp3=max(aa,max(sum[rr-1]-sum[i-1],sum[n]-sum[rr-1]));
			LL tmp=min(tmp1,min(tmp2,tmp3));
			ans=max(ans,sum[n]-tmp);
		}
		printf("Case #%d: %.9lf\n",++tt,ans*1.0/sum[n]);
	}
	return 0;
}