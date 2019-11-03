#include <vector>
#include <list>
#include <map>
#include <set>
#include <deque>
#include <stack>
#include <bitset>
#include <algorithm>
#include <functional>
#include <numeric>
#include <utility>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>
 
using namespace std;

#define GMAX(X, Y) ((X) > (Y) ? (X) : (Y))
#define GMIN(X, Y) ((X) < (Y) ? (X) : (Y))
#define RP(a,h) for(a=0; a<(h); a++)
#define FR(a,l,h) for(a=(l); a<=(h); a++)
#define SZ(a) (LL)a.size()
#define ALL(a) a.begin(), a.end()
#define pb push_back
typedef vector <int> VI;
typedef vector <string> VS;
typedef pair<int, int> PII;
#define LL long long
#define INF 1000000

int hx[4]={-1, 0, 1, 0};
int hy[4]={0, 1, 0, -1};

int L, T;
string S, way;
VI x, y;
int nline;
VI ax, ay;

void makeXY()
{
	x.clear();
	y.clear();
	ax.clear();
	ay.clear();
	int i, j;
	int px=0;
	int py=0;
	int pd=0;
	x.pb(px); y.pb(py);
	ax.pb(px); ay.pb(py);
	nline=0;
	RP(i, SZ(way))
	{
		if (way[i]=='L') { pd=(pd+3)%4; continue; }
		if (way[i]=='R') { pd=(pd+1)%4; continue; }
		FR(j, i+1, SZ(way))
		{
			if (j==SZ(way)) break;
			if (way[j]!='F') break;
		}
		int nx=px+hx[pd]*(j-i);
		int ny=py+hy[pd]*(j-i);
		x.pb(nx); y.pb(ny);
		ax.pb(nx); ay.pb(ny);
		nline++;
		i=j-1;
		px=nx;
		py=ny;
	}
	sort(ALL(ax));
	sort(ALL(ay));
	ax.erase(unique(ALL(ax)), ax.end());
	ay.erase(unique(ALL(ay)), ay.end());
}

int countRight(int x1, int x2, int y1, int y2)
{
	int i;
	int res=0;
	RP(i, nline)
	{
		if (y[i]==y[i+1] && y[i]*2>(y1+y2))
		{
			int tx=GMIN(x[i], x[i+1]);
			int bx=GMAX(x[i], x[i+1]);
			if (tx*2<=(x1+x2) && (x1+x2)<=bx*2)
				res++;
		}
	}
	return res;
}

int countLeft(int x1, int x2, int y1, int y2)
{
	int i;
	int res=0;
	RP(i, nline)
	{
		if (y[i]==y[i+1] && y[i]*2<(y1+y2))
		{
			int tx=GMIN(x[i], x[i+1]);
			int bx=GMAX(x[i], x[i+1]);
			if (tx*2<=(x1+x2) && (x1+x2)<=bx*2) res++;
		}
	}
	return res;
}

int countDown(int x1, int x2, int y1, int y2)
{
	int i;
	int res=0;
	RP(i, nline)
	{
		if (x[i]==x[i+1] && x[i]*2>(x1+x2))
		{
			int ty=GMIN(y[i], y[i+1]);
			int by=GMAX(y[i], y[i+1]);
			if (ty*2<=(y1+y2) && (y1+y2)<=by*2)
				res++;
		}
	}
	return res;
}

int countUp(int x1, int x2, int y1, int y2)
{
	int i;
	int res=0;
	RP(i, nline)
	{
		if (x[i]==x[i+1] && x[i]*2<(x1+x2))
		{
			int ty=GMIN(y[i], y[i+1]);
			int by=GMAX(y[i], y[i+1]);
			if (ty*2<=(y1+y2) && (y1+y2)<=by*2)
				res++;
		}
	}
	return res;
}

bool isPocket(int x1, int x2, int y1, int y2)
{
	int cntR=countRight(x1, x2, y1, y2);
	if (cntR%2==1) return false;
	int cntL=countLeft(x1, x2, y1, y2);
	if (cntR>0 && cntL>0) return true;

	int cntD=countDown(x1, x2, y1, y2);
	int cntU=countUp(x1, x2, y1, y2);
	if (cntD>0 && cntU>0) return true;

	return false;
}

int main()
{
	freopen("Round 3\\A\\A-small-attempt0.in", "r", stdin);
	freopen("Round 3\\A\\A-small-attempt0.out", "w", stdout);

	int numtest, test, i, j;
	cin >> numtest;

	RP(test, numtest)
	{
		cin >> L;
		way="";
		RP(i, L)
		{
			cin >> S >> T;
			RP(j, T) way += S;
		}
		makeXY();
		int ans=0;
		RP(i, SZ(ax)-1) RP(j, SZ(ay)-1) if (isPocket(ax[i], ax[i+1], ay[j], ay[j+1]))
			ans+=(ax[i+1]-ax[i]) * (ay[j+1]-ay[j]);
		cout << "Case #" << (test+1) << ": " << ans << endl;
	}
	return 0;
}
