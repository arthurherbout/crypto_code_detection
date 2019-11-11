#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <set>
#include <map>
#include <cstring>

using namespace std;

typedef long long LL;
typedef unsigned long long ULL;

#define SIZE(x) (int((x).size()))
#define rep(i,l,r) for (int i=(l); i<=(r); i++)
#define repd(i,r,l) for (int i=(r); i>=(l); i--)
#define rept(i,c) for (typeof((c).begin()) i=(c).begin(); i!=(c).end(); i++)

#ifndef ONLINE_JUDGE
#define debug(x) { cerr<<#x<<" = "<<(x)<<endl; }
#else
#define debug(x) {}
#endif

#define maxn 1000010

int n;
LL a[maxn], s[maxn];
LL ans;

int check(int l, int r)
{
	LL t=s[r]-s[l-1];
	t=max(t,s[l-1]);
	t=max(t,s[n]-s[r]);
	ans=min(ans,t);
}

void lemon()
{
	int _p,_q,_r,_s; scanf("%d%d%d%d%d",&n,&_p,&_q,&_r,&_s);
	rep(i,1,n) a[i]=(LL(i-1)*_p+_q)%_r+_s;
	s[0]=0;
	rep(i,1,n) s[i]=s[i-1]+a[i];
	
	ans=1000000000000000000;
	rep(i,1,n)
	{
		int left=0, right=i;
		while (left!=right)
		{
			int mid=(left+right+1)/2;
			if (s[mid]<=s[i]-s[mid]) left=mid; else right=mid-1;
		}
		if (left+1<=i) check(left+1,i);
		if (left+2<=i) check(left+2,i);
	}
	printf("%.16lf\n",double(s[n]-ans)/s[n]);
}

int main()
{
	ios::sync_with_stdio(true);
	#ifndef ONLINE_JUDGE
		freopen("A.in","r",stdin);
	#endif
	int tcase; scanf("%d",&tcase);
	rep(nowcase,1,tcase) 
	{
		printf("Case #%d: ",nowcase);
		lemon();
	}
	return 0;
}

