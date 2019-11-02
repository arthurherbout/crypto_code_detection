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
#define X first
#define Y second
#define PB push_back
#define FOR(i,a,b) for( int i=(a); i<(b); ++i)
#define FORD(i,a,b) for( int i=(a); i>(b); --i)
#define REP(i,n) for(int i=0; i<(n); ++i)
#define ALL(X) (X).begin(),(X).end()
#define SZ(X) (int)(X).size()
#define FORE(it,X) for(__typeof((X).begin()) it=(X).begin(); it!=(X).end(); ++it)

int ccw(int x1,int y1,int x2,int y2,int x3,int y3)
{
	return x1*y2-x2*y1+x2*y3-x3*y2+x3*y1-x1*y3;
}

int chk[6005][6005];
int hy[4]={-1,0,1,0};
int hx[4]={0,1,0,-1};

int main()
{
	int tn,qq=0;

	cin>>tn;
	while (tn--) {
		int n;
		cin>>n;
		string dt;
		while (n--) {
			string s;
			int r;
			cin>>s>>r;
			while (r--)
				dt+=s;
		}
		memset(chk,0,sizeof(chk));
		chk[3000][3000]=1;
		int go=0;
		int y,x;
		int ymin,ymax;
		int xmin,xmax;

		ymin=xmin=6005;
		ymax=xmax=-5;

		y=x=3000;
		VPII da;
		da.PB(MP(y,x));
		REP(i,SZ(dt)) {
			if (dt[i]=='F') {
				y+=hy[go];
				x+=hx[go];
				chk[y][x]=1;
				ymin=min(ymin,y);
				xmin=min(xmin,x);
				ymax=max(ymax,y);
				xmax=max(xmax,x);
			}
			else if (dt[i]=='R') {
				da.PB(MP(y,x));
				go++;
				go%=4;
			}
			else {
				da.PB(MP(y,x));
				go+=3;
				go%=4;
			}
		}
		
		int area=0;
		FOR(i,1,SZ(da)-1)
			area+=ccw(da[0].X,da[0].Y,da[i].X,da[i].Y,da[i+1].X,da[i+1].Y);
		area=abs(area)/2;

//		cout<<"area : "<<area<<endl;

/*		FOR(i,80,121) {
		FOR(j,80,121)
			cout<<chk[i][j];
		cout<<endl;
		}*/

		FOR(i,ymin,ymax+1) {
			int x1,x2;
			for (x1=xmin;x1<=xmax;x1++)
				if (chk[i][x1])
					break;
			for (x2=xmax;x2>=xmin;x2--)
				if (chk[i][x2])
					break;
			while (x1<=x2)
				chk[i][x1++]=1;
		}
		FOR(i,xmin,xmax+1) {
			int x1,x2;
			for (x1=ymin;x1<=ymax;x1++)
				if (chk[x1][i])
					break;
			for (x2=ymax;x2>=ymin;x2--)
				if (chk[x2][i])
					break;
			while (x1<=x2)
				chk[x1++][i]=1;
		}
/*		FOR(i,80,121) {
		FOR(j,80,121)
			cout<<chk[i][j];
		cout<<endl;
		}*/
		printf("Case #%d: ",++qq);

		int area2=0;
		FOR(i,ymin,ymax+1)
		FOR(j,xmin,xmax+1)
			if (chk[i][j] && chk[i+1][j] && chk[i][j+1] && chk[i+1][j+1])
				area2++;
		cout<<area2-area<<endl;
	}
	return 0;
}
