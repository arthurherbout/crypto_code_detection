#include <cstdio>
#include <algorithm>
#include <vector>
#include <iostream>
#include <string>

using namespace std;

typedef long long LL;
typedef long double LD;
typedef vector<int> VI;
typedef pair<int,int> PII;
typedef vector<PII> VPII;

#define MP make_pair
#define ST first
#define ND second
#define PB push_back
#define FOR(i,a,b) for( int i=(a); i<(b); ++i)
#define FORD(i,a,b) for( int i=(a); i>(b); --i)
#define REP(i,n) for(int i=0; i<(n); ++i)
#define ALL(X) (X).begin(),(X).end()
#define SZ(X) (int)(X).size()
#define FORE(it,X) for(__typeof((X).begin()) it=(X).begin(); it!=(X).end(); ++it)

#include<cmath>

int pre[]={527,743,351,135,407,903,791,135,647};

int main()
{
	int tn,qq=0;

	cin>>tn;
	int n=1;
	while (tn--) {
		int n;

		cin>>n;

		int dp;
		if (n<=21) {
			LD d=3.0+sqrtl(5.0);
			d=powl(d,(LD)n);
			d=fmodl(d,1000.0);
			d=floorl(d);
			dp=(int)(d+0.1);
		}
		else
			dp=pre[n-22];
		printf("Case #%d: ",++qq);
		printf("%03d\n",dp);
//		n++;
	}
	return 0;
}
