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

#define maxn 1010

int a[maxn], flag[maxn];

void lemon()
{
	int n; scanf("%d",&n);
	rep(i,1,n) scanf("%d",&a[i]), a[i]++;
	memset(flag,0,sizeof flag);
	int pd=1; double xs=1;
	rep(t,1,n)
		if (!flag[t])
		{
			int x=1, lastsign=0, sc=0, cc=0;
			while (!flag[x])
			{
				if (a[x]>x)
				{
					if (lastsign==0) sc++, xs*=sc; else sc=1, lastsign=0;
				}
				else  
				{
					if (lastsign==1) sc++, xs/=sc; else sc=1, lastsign=1;
				}
				flag[x]=1; x=a[x]; 
				//cc++; xs*=cc;
			}
		}
	if (xs<=1-1e-8) printf("GOOD\n"); else printf("BAD\n");
}

int main()
{
	ios::sync_with_stdio(true);
	#ifndef ONLINE_JUDGE
		freopen("C.in","r",stdin);
	#endif
	int tcase; scanf("%d",&tcase);
	rep(nowcase,1,tcase)
	{
		printf("Case #%d: ",nowcase);
		lemon();
	}
	return 0;
}

