
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

int n,c;
double dy[45][45];
double cc[45][45];

int main()
{
	int tn,qq=1;

	cc[0][0]=1;
	REP(i,41) REP(j,41) {
		cc[i+1][j]+=cc[i][j];
		cc[i+1][j+1]+=cc[i][j];
	}
	cin>>tn;
	while (tn--) {
		cin>>n>>c;
		memset(dy,0,sizeof(dy));

		int b;
		double p;
		REP(tt,10000) {
//			cout<<tt<<"th loop"<<endl;
			REP(j,n) {
				double v=dy[n][j];
				dy[n][j]=0;
				REP(a,j+1) {
					b=c-a;
					if (n-j<b) continue;
					p=cc[j][a]*cc[n-j][b]/cc[n][c];
					if (b==0)
						dy[n][j]+=(v+1)*p;
					else
						dy[n][j]+=(dy[n][j+b]+1)*p;
					//cout<<n<<", "<<j<<" : "<<dy[n][j]<<endl; char qqq[5]; gets(qqq);
				}
			}
		}
		printf("Case #%d: %.7lf\n",qq++,dy[n][0]);
	}
	return 0;
}
