#include <vector>
#include <map>
#include <set>
#include <stack>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <cstdio>
#include <cmath>
#include <string>
#include <cctype>
#include <cstring>
#include <queue>
#include <cassert>
#include <ctime>

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
#define FORE(it,X) for(__typeof((X).begin()) it=(X).begin(); it!=(X).end();++it)

vector<LL> su;
bool chk[1024];
int dp;
int n;

void process()
{
	dp =0;

	vector<int> e(SZ(su),0);
	vector<int> v(SZ(su),0);

	FOR(i,2,n+1) {
		int s=i;
		REP(j,SZ(su)) {
			int w=0;
			while(s%su[j]==0) {
				s/=su[j];
				w++;
			}
			e[j]=max(e[j],w);
		}
	}

	int mx=1,mn=0;
	REP(i,SZ(su)) {
		mx+=e[i];
		if (e[i])
			mn++;
	}
	
	/*
	int mn=0;
	FORD(i,n,0) {
		int s=i;
		bool plus=false;
		REP(j,SZ(su)) {
			int w=0;
			while(s%su[j]==0) {
				s/=su[j];
				w++;
			}
			if (v[j]<w) {
				v[j]=w;
				plus=true;
			}
		}
		mn+=plus;
	}
*/
	dp=mx-mn;
}

int main()
{
	for (LL i=2;i<=1000;i++) {
		if (!chk[i]) {
			su.PB(i);
			for(LL j=i;j<=1000;j+=i) {
				chk[j]=true;
			}
//			cout<<i<<endl;
		}
	}

	int tn;
	cin>>tn;
	while (tn--) {
		cin>>n;
		process();
		static int qq=0;
		if (n==1) dp=0;
		printf("Case #%d: %d\n",++qq,dp);
	}
	return 0;
}
