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
#define SQ(a) ((a)*(a))
#define DIS(a,b) ( sqrt((double)SQ( (a).x-(b).x )+ SQ( (a).y-(b).y )) )

struct ele {
    double x,y,r;
};

int n;
ele dt[3];

int main()
{
	int tn;

	cin>>tn;
	FOR(qq,1,tn+1) {
		cin>>n;
		REP(i,n)
			cin>>dt[i].x>>dt[i].y>>dt[i].r;

		printf("Case #%d: ",qq);
		if (n==1) {
			printf("%.6lf\n",dt[0].r);
			continue;
		}
		else if (n==2) {
			printf("%.6lf\n",max(dt[0].r,dt[1].r));
			continue;
		}

		double dp=1e100;
		REP(i,n) FOR(j,i+1,n) {
			dp=min(dp,DIS(dt[i],dt[j])+dt[i].r+dt[j].r);
		}

		printf("%.6lf\n",dp/2.0);
	}
	return 0;
}
