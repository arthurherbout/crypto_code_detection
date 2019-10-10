#include <algorithm>
#include <cstdio>
#include <iostream>
#include <utility>
#include <queue>
using namespace std;

const int MaxR = 10 + 10;
const int MOD = 10007;
const int MaxN = 100 + 10;
int totCase, caseNum;

int n, r;
pair <int, int> p[MaxR];
int f[MaxN][MaxN];
bool v[MaxN][MaxN];
bool g[MaxN][MaxN];

int w, h;

int work();
int bfs();

int main() {
	scanf("%d", &totCase);
	for (caseNum = 1; caseNum <= totCase; ++caseNum) {
		scanf("%d %d %d", &h, &w, &r);
		for (int i = 0; i <= h; ++i)
			for (int j = 0; j <= w; ++j)
				g[i][j] = true;
		for (int i = 0; i < r; ++i) {
			scanf("%d %d", &p[i].first, &p[i].second);
			g[p[i].first][p[i].second] = false;
		}
		printf("Case #%d: %d\n", caseNum, work());
	}
	return 0;
}

int work() {
	return bfs();
}

int bfs() {
	queue <pair <int, int> > q;
	q.push(make_pair(1, 1));
	for (int i = 0; i <= h; ++i)
		for (int j = 0; j <= w; ++j) {
			v[i][j] = false;
			f[i][j] = 0;
		}
	v[1][1] = true;
	f[1][1] = 1;
	while (!q.empty()) {
		pair <int, int> cur = q.front();
		v[cur.first][cur.second] = false;
		q.pop();
		int x = cur.first + 1, y = cur.second + 2;
		if (x <= h && y <= w && g[x][y]) {
			f[x][y] += f[cur.first][cur.second];
			f[x][y] %= MOD;
			if (!v[x][y]) {
				q.push(make_pair(x, y));
				v[x][y] = true;
			}
		}
		x = cur.first + 2, y = cur.second + 1;
		if (x <= h && y <= w && g[x][y]) {
			f[x][y] += f[cur.first][cur.second];
			f[x][y] %= MOD;
			if (!v[x][y]) {
				q.push(make_pair(x, y));
				v[x][y] = true;
			}
		}
	}
	return f[h][w];
}



