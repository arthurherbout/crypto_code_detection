#include <iostream>
#include <climits>
#include <cstdio>
#include <queue>

using namespace std;

int dx[] = {0, 0, 1, -1};
int dy[] = {1, -1, 0, 0};
int d[200][200];
int c[200][200];
int f[200][200];

struct Pos {
	int h, x, y;
	Pos(int _h=0, int _x=0, int _y=0) {
		h = _h;
		x = _x;
		y = _y;
	}
};

bool operator<(const Pos&pos1, const Pos&pos2) {
	return pos1.h<pos2.h;
}

int check(int f1, int c1, int f2, int c2) {
	int f = max(f1, f2);
	int c = min(c1, c2);
	if(f+50>c) return INT_MIN/2;
	return c-50;
}

int main() {
	int T;
	scanf("%d", &T);
	for(int cas=1; cas<=T; ++cas) {
		int n, m, h;
		scanf("%d", &h);
		scanf("%d%d", &n, &m);
		for(int i=1; i<=n; ++i)
			for(int j=1; j<=m; ++j)
				scanf("%d", c[i]+j);
		for(int i=1; i<=n; ++i)
			for(int j=1; j<=m; ++j)
				scanf("%d", f[i]+j);
		priority_queue<Pos> q;
		for(int i=1; i<=n; ++i)
			for(int j=1; j<=m; ++j)
				d[i][j] = INT_MIN/2;
		d[1][1] = h;
		q.push(Pos(h, 1, 1));
		while(!q.empty()) {
			Pos pos = q.top();
			q.pop();
			if(d[pos.x][pos.y]!=pos.h) continue;
			for(int w=0; w<4; ++w) {
				Pos nxt = pos;
				nxt.x += dx[w];
				nxt.y += dy[w];
				if(nxt.x<=0) continue;
				if(nxt.x> n) continue;
				if(nxt.y<=0) continue;
				if(nxt.y> m) continue;
				int sh = check(	f[pos.x][pos.y],
						c[pos.x][pos.y],
						f[nxt.x][nxt.y],
						c[nxt.x][nxt.y]
					      );
				sh = min(sh, pos.h);
				if(f[pos.x][pos.y]+20<=sh)
					nxt.h = sh-10;
				else 
					nxt.h = sh-100;
				if(sh==h) nxt.h = h;
				if(d[nxt.x][nxt.y]>=nxt.h) continue;
				d[nxt.x][nxt.y] = nxt.h;
				q.push(nxt);
			}
		}
		printf("Case #%d: %.2lf\n", cas, (h-d[n][m])/10.0);
	}
}
