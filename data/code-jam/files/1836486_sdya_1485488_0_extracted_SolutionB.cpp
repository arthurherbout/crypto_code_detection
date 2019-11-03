#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <bitset>
#include <cstring>
#include <string>
#include <queue>
#include <stack>
#include <set>
#include <map>
#include <ctime>
#pragma comment (linker, "/STACK:256000000")

using namespace std;

const int maxN = 110;

int h, n, m;
int c[maxN][maxN], f[maxN][maxN];
int d[12][12][31000];

bool val(int x, int y) {
	return 1 <= x && x <= n && 1 <= y && y <= m;
}

int dx[] = {-1, 0, 0, 1};
int dy[] = { 0,-1, 1, 0};

bool used[maxN][maxN];

void dfs(int x, int y) {
	if (used[x][y]) return ;
	used[x][y] = true;

	for (int i = 0; i < 4; ++i) {
		int nx = x + dx[i], ny = y + dy[i];
		if (!val(nx, ny)) continue;

		int f1 = max(h, f[x][y]), f2 = max(h, f[nx][ny]);
		int c1 = c[x][y], c2 = c[nx][ny];

		if (c2 - f1 >= 50 && c1 - f2 >= 50 && c2 - f2 >= 50) {
			dfs(nx, ny);
		}
	}
}

void solve(int test) {
	scanf("%d%d%d", &h, &n, &m);
	for (int i = 1; i <= n; ++i)
		for (int j = 1; j <= m; ++j)
			scanf("%d", &c[i][j]);
	for (int i = 1; i <= n; ++i)
		for (int j = 1; j <= m; ++j)
			scanf("%d", &f[i][j]);

	for (int i = 1; i <= n; ++i)
		for (int j = 1; j <= m; ++j) {
			used[i][j] = false;
			for (int k = 0; k < 31000; ++k) {
				d[i][j][k] = 0;
			}
		}

	int u = 0, v = 1;
	if (c[1][1] - max(h, f[1][1]) >= 50) {
		dfs(1, 1);
		for (int i = 1; i <= n; ++i)
			for (int j = 1; j <= m; ++j)
				if (used[i][j]) {
					d[i][j][0] = 1;
				}
	} else {
		int need = h - (c[1][1] - 50);
		d[1][1][need] = 1;
	}

	for (int step = 0; step < 30000; ++step, --h) {
		for (int i = 1; i <= n; ++i)
			for (int j = 1; j <= m; ++j)
				if (d[i][j][step]) {
					if (step + 1 < 30000) {
						d[i][j][step + 1] = 1;
					}
					for (int k = 0; k < 4; ++k) {
						int x = i, y = j;
						int nx = x + dx[k], ny = y + dy[k];
						if (!val(nx, ny)) continue;

						int f1 = max(h, f[x][y]), f2 = max(h, f[nx][ny]);
						int c1 = c[x][y], c2 = c[nx][ny];

						if (c2 - f1 >= 50 && c1 - f2 >= 50 && c2 - f2 >= 50) {
							if (h >= f[x][y] + 20) {
								if (step + 10 < 30000) {
									d[nx][ny][step + 10] = 1;
								}
							} else {
								if (step + 100 < 30000) {
									d[nx][ny][step + 100] = 1;
								}
							}
						}
					}
				}
		if (d[n][m][step]) {
			printf("Case #%d: %d.%d\n", test, step / 10, step % 10);
			break;
		}
	}
}

int main() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	int tests;
	scanf("%d", &tests);
	for (int i = 1; i <= tests; ++i) {
		solve(i);
		cerr << i << endl;
	}
	return 0;
}