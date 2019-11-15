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

int chk[105][105];

int main()
{
	int qq=0;
	int tn;

	cin>>tn;
	while (tn--) {
		int n,m;
		cin>>n>>m;
		REP(i,m) REP(j,n) chk[i][j]=-1;

		REP(i,m) {
			int ka;
			cin>>ka;
			while (ka--) {
				int a,b;
				cin>>a>>b;
				a--;
				chk[i][a]=b;
			}
		}

/*		REP(i,m) {
			REP(j,n) cout<<chk[i][j]<<" ";
			cout<<endl;
		}*/

		VI perm(n),dp;
		REP(i,1<<n) {
			REP(j,n) perm[j]=( (i&(1<<j)) >0) ? 1 : 0;
			
//			REP(j,n) cout<<perm[j]<<" "; cout<<endl;

			int good=1;
			REP(j,m) {
				int ch=0;
				REP(k,n)
					if (chk[j][k]==perm[k]) {
						ch=1;
						break;
					}
				if (!ch) {
					good=0;
					break;
				}
			}
		
			if (!good) continue;

			if (SZ(dp)==0) dp=perm;
			else if (count(ALL(dp),1)>count(ALL(perm),1)) dp=perm;
		}

		printf("Case #%d:",++qq);
		if (SZ(dp)==0) printf(" IMPOSSIBLE\n");
		else {
			REP(i,n) printf(" %d",dp[i]);
			printf("\n");
		}
	}
	return 0;
}

/*
2
5
3
1 1 1
2 1 0 2 0
1 5 0
1
2
1 1 0
1 1 1
 Case #1: 1 0 0 0 0
 Case #2: IMPOSSIBLE 

 */
