#include <vector>
#include <list>
#include <map>
#include <set>
#include <deque>
#include <queue>
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

#define  ABS(X) ( ((X) > 0) ? (X) : -(X) )
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
typedef pair<PII, PII> P4;
#define LL long long
#define INF 1000000

int hx[4]={-1, 0, 1, 0};
int hy[4]={0, 1, 0, -1};

int H, W;
int bx, by, ex, ey;
int cache[36][36];
VS a;

void forward(int x, int y, int d, int &kx, int &ky)
{
	int px=x, py=y;
	while (1)
	{
		int nx=px+hx[d];
		int ny=py+hy[d];
		if (nx<0||nx>=H||ny<0||ny>=W) break;
		if (a[nx][ny]=='#') break;
		px=nx;
		py=ny;
	}
	kx=px;
	ky=py;
}

void bfs()
{
	queue<int> qx, qy, qd;
	qx.push(bx);
	qy.push(by);
	qd.push(0);
	while (!qx.empty())
	{
		int cx=qx.front(); qx.pop();
		int cy=qy.front(); qy.pop();
		int cd=qd.front(); qd.pop();
		int i, j, nx, ny;
		RP(i, 4)
		{
			nx=cx+hx[i];
			ny=cy+hy[i];
			if (nx<0||nx>=H||ny<0||ny>=W) continue;
			if (a[nx][ny]=='#') continue;
			if (cache[nx][ny]!=-1 && cache[nx][ny]<=cd+1) continue;
			qx.push(nx);
			qy.push(ny);
			qd.push(cd+1);
			cache[nx][ny]=cd+1;
		}
		int x[4], y[4], tothere[4];
		RP(i, 4) forward(cx, cy, i, x[i], y[i]);
		RP(i, 4) tothere[i]=ABS(x[i]-cx)+ABS(y[i]-cy);
		RP(i, 4) RP(j, 4) if (i!=j)
		{
			int val=tothere[i]+1;
			nx=x[j];
			ny=y[j];
			if (nx<0||nx>=H||ny<0||ny>=W) continue;
			if (a[nx][ny]=='#') continue;
			if (cache[nx][ny]!=-1 && cache[nx][ny]<=cd+val) continue;
			qx.push(nx);
			qy.push(ny);
			qd.push(cd+val);
			cache[nx][ny]=cd+val;
		}
	}
}
 
int main()
{
	freopen("Round 3\\B\\B-large.in", "r", stdin);
	freopen("Round 3\\B\\B-large.out", "w", stdout);

	int numtest, test, i, j;
	cin >> numtest;

	RP(test, numtest)
	{
		cin >> H >> W;
		a.resize(H);
		RP(i, H) cin >> a[i];
		RP(i, H) RP(j, W)
		{
			if (a[i][j]=='O') {a[i][j]='.'; bx=i; by=j;}
			if (a[i][j]=='X') {a[i][j]='.'; ex=i; ey=j;}
		}
		memset(cache, -1, sizeof(cache));
		bfs();
		int ans=cache[ex][ey];
		if (ans==-1) cout << "Case #" << (test+1) << ": THE CAKE IS A LIE" << endl;
		else cout << "Case #" << (test+1) << ": " << ans << endl;
	}
	return 0;
}
