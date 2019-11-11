#include <vector>
#include <map>
#include <set>
#include <stack>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <cstdio>
#include <string>
#include <queue>
#include <cmath>

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

int dt[8][8],db[8][8];
int n;

int ret()
{
	REP(i,n) REP(j,n) if (dt[i][j]==0 && db[i][j]==1) return 0;
	return 1;
}

int main()
{
	int tn;

	cin>>tn;
	FOR(qq,1,tn+1) {
		int m;
		cin>>n;
		memset(dt,0,sizeof(dt));
		REP(i,n-1) {
			int a,b;
			cin>>a>>b;
			a--,b--;
			dt[a][b]=dt[b][a]=1;
		}
		
		cin>>m;
		VPII da(m);
		REP(i,m-1) {
			cin>>da[i].ST>>da[i].ND;
			da[i].ST--;
			da[i].ND--;
		}

		VI chk(n);
		REP(i,n) chk[i]=i;

		int yes=0;
		do {
			memset(db,0,sizeof(db));
			REP(i,m-1) {
				int v=chk[da[i].ST];
				int u=chk[da[i].ND];
				db[v][u]=db[u][v]=1;
			}
			if (ret()) { yes=1; break; }
		} while(next_permutation(ALL(chk)));
		
/*		if (yes) {
			REP(i,m)
				cout<<chk[i]<<" "; cout<<endl;
		}*/
		printf("Case #%d: ",qq);
		if (yes) cout<<"YES"<<endl;
		else cout<<"NO"<<endl;
	}
	return 0;
}
