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

int dt[105][105];
int dy[105][105];

int main()
{
	int tn,qq=0;

	cin>>tn;
	while (tn--) {
		int h,w,R;
		int r,c;

		cin>>h>>w>>R;
		memset(dy,0,sizeof(dy));
		memset(dt,0,sizeof(dt));
		while (R--) {
			cin>>r>>c;
			r--,c--;
			dt[r][c]=1;
		}
		dy[0][0]=1;
		REP(i,h)
		REP(j,w) {
			if (dt[i][j]) continue;
			int y,x;
			y=i+2;
			x=j+1;
			if (y<h && x<w) {
				dy[y][x]+=dy[i][j];
				dy[y][x]%=10007;
			}
			y=i+1;
			x=j+2;
			if (y<h && x<w) {
				dy[y][x]+=dy[i][j];
				dy[y][x]%=10007;
			}
		}

/*		REP(i,h) {
		REP(j,w)
			cout<<dy[i][j];
		cout<<endl;
		}*/
		printf("Case #%d: ",++qq);
		printf("%d\n",dy[h-1][w-1]);
	}
	return 0;
}
