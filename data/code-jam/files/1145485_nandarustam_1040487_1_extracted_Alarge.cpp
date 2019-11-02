#include <cstdio>
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <cstring>
#include <queue>
#include <complex>
#include <cmath>
#include <map>
using namespace std;

#define REP(i,n) for(int i = 0; i < n; ++i)
#define FOR(i,a,b) for(int i = a; i <= b; ++i)
#define FORd(i,a,b) for(int i = a; i >= b; --i)
#define mset(ar,x) memset(ar,x,sizeof(ar))
#define pub push_back
#define fr first
#define sc second
#define mp make_pair

typedef long long llong;
llong tc,n,pd,pg;
llong gall,dall,gwin,glose,dwin,dlose;

int main()
{
	scanf("%lld",&tc);
	FOR(t,1,tc){
		scanf("%lld %lld %lld",&n,&pd,&pg);
		gall = 100; gwin = pg;
		llong x = n/gall + 1;
		gall *= x;  gwin *= x;
		glose = gall-gwin;
		llong up = pd, dn = 100;
		llong fpb = __gcd(up,dn);
		up /= fpb;
		dn /= fpb;
		bool yes = 1;
		if(dn > n) yes = 0;
		else{
			dall = dn;
			dwin = up;
			dlose = dn-up;
			if(gwin < dwin) yes = 0;
			if(glose < dlose) yes = 0;
		}
		
		if(yes) printf("Case #%d: Possible\n",t);
		else printf("Case #%d: Broken\n",t);
	}
	
	return 0;
}


