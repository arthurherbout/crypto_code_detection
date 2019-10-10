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

int ret(string &a)
{
	int v=1;
	
	FOR(i,1,SZ(a))
		if (a[i]!=a[i-1])
			v++;
	return v;
}

int main()
{
	int tn,qq=0;

	cin>>tn;
	while (tn--) {
		int k;
		string dt;
		cin>>k>>dt;
		VI per(k);
		REP(i,k) per[i]=i;

		int dp=234234234;
		do {
			string p=dt;
			for (int i=0;i<SZ(dt);i+=k)
				REP(j,k)
					p[i+j]=dt[i+per[j]];

//			cout<<p<<endl;

			dp=min(dp,ret(p));
		} while(next_permutation(ALL(per)));

		printf("Case #%d: ",++qq);
		cout<<dp<<endl;
	}
	return 0;
}
