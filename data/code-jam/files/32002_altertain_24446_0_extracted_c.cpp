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

int possible(int a)
{
	int one=0;
	while (a) {
		if (a&1) {
			if (one) return 0;
			one=1;
		}
		else one=0;
		a>>=1;
	}
	return 1;
}

int count(int a)
{
	int h=0;

	while (a) {
		if (a&1) h++;
		a>>=1;
	}
	return h;
}

char dt[15][15];
int dy[15][2048];

int main()
{
	int tn,qq=0;

	cin>>tn;
	while (tn--) {
		int n,m;

		cin>>n>>m;
		REP(i,n)
			scanf("%s",dt[i]);
		
		memset(dy,0,sizeof(dy));
		int dp=0;
		REP(i,n) 
		REP(j,1<<m) {
			if (possible(j)==0) continue;
			int no=0;
			REP(k,m)
				if (j&(1<<k))
					if (dt[i][k]=='x') {
						no=1;
						break;
					}
			if (no) continue;
			int cnt=count(j);
			if (i==0) {
				dy[i][j]=cnt;
				dp=max(dp,dy[i][j]);
				continue;
			}
			REP(k,1<<m) {
				if (possible(k)==0) continue;
				int su=j|k;
				if (possible(su)==0) continue;
				dy[i][j]=max(dy[i][j],dy[i-1][k]+cnt);
				dp=max(dp,dy[i][j]);
			}
		}
		printf("Case #%d: ",++qq);
		printf("%d\n",dp);
	}
	return 0;
}
