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

struct ele {
	int y,x,c;
	
	ele() {}
	ele(int _y,int _x,int _c)
	{
		y=_y;
		x=_x;
		c=_c;
	}

	bool operator<(ele t) const
	{
		return c>t.c;
	}
};

int s[15][15],w[15][15],t[15][15];
bool gs[15][15][150],gw[15][15][150];
int n,m;
int dy[15][15];
priority_queue<ele> pq;

void go(int y,int x,int c)
{
	if (y<0 || y>=2*n || x<0 || x>=2*m) return;
	if (dy[y][x]<=c) return;

	dy[y][x]=c;
	pq.push(ele(y,x,c));
}

int main()
{
	int tn,qq=0;

	cin>>tn;
	while (tn--) {
		cin>>n>>m;
		FORD(i,n-1,-1) REP(j,m) {
			cin>>s[i][j]>>w[i][j]>>t[i][j];
			REP(k,150) {
				int q=(k-t[i][j])%(s[i][j]+w[i][j]);
				while (q<0) q+=(s[i][j]+w[i][j]);
				if (q<s[i][j]) gs[i][j][k]=true,gw[i][j][k]=false;
				else gs[i][j][k]=false,gw[i][j][k]=true;
			}
		}
		
/*		REP(k,150)
			if (gs[0][0][k]) cout<<"T";
			else cout<<"F";
		cout<<endl;
*/
		memset(dy,63,sizeof(dy));
		dy[0][0]=0;		
		pq.push(ele(0,0,0));

		while (!pq.empty()) {
			int i,j,p;
			i = pq.top().y;
			j = pq.top().x;
			p = pq.top().c; pq.pop();
			if (dy[i][j]!=p) continue;

//			cout<<i<<" "<<j<<" "<<p<<endl; char qqq[5]; gets(qqq);

			int y,x;
			y=i/2;
			x=j/2;

			if (i&1) {
				go(i+1,j,p+2);
				FOR(next,p,150) if (gs[y][x][next]) {
					go(i-1,j,next+1);
					break;
				}
			}
			else {
				go(i-1,j,p+2);
				FOR(next,p,150) if (gs[y][x][next]) {
					go(i+1,j,next+1);
					break;
				}
			}

			if (j&1) {
				go(i,j+1,p+2);
				FOR(next,p,150) if (gw[y][x][next]) {
					go(i,j-1,next+1);
					break;
				}
			}
			else {
				go(i,j-1,p+2);
				FOR(next,p,150) if (gw[y][x][next]) {
					go(i,j+1,next+1);
					break;
				}
			}
		}

		printf("Case #%d: %d\n",++qq,dy[2*n-1][2*m-1]);
	}
	return 0;
}
