#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <cstring>
#include <string>
#include <queue>
#include <set>
#include <map>
#include <bitset>
#include <cmath>
#include <ctime>
#pragma comment (linker, "/STACK:256000000")

using namespace std;

void readData() {
}

const int maxN = 2000;
int n, m;
int p[maxN];
pair < int, int > c[maxN];
int position[maxN];
char maze[maxN][maxN];

bool used[maxN][maxN][4];

void set0() {
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			for (int k = 0; k < 4; ++k) {
				used[i][j][k] = false;
			}
		}
	}
}

void dfs(int x, int y, int p) {
	if (x < 0 || x >= n || y < 0 || y >= m) {
		return;
	}
	if (used[x][y][p]) {
		return;
	}
	used[x][y][p] = true;
	if (p == 0) {
		dfs(x - 1, y, 2);
		if (maze[x][y] == '/') {
			dfs(x, y, 3);
		} else {
			dfs(x, y, 1);
		}
	}
	if (p == 1) {
		dfs(x, y + 1, 3);
		if (maze[x][y] == '/') {
			dfs(x, y, 2);
		} else {
			dfs(x, y, 0);
		}
	}
	if (p == 2) {
		dfs(x + 1, y, 0);
		if (maze[x][y] == '/') {
			dfs(x, y, 1);
		} else {
			dfs(x, y, 3);
		}
	}
	if (p == 3) {
		dfs(x, y - 1, 1);
		if (maze[x][y] == '/') {
			dfs(x, y, 0);
		} else {
			dfs(x, y, 2);
		}
	}
}

bool check() {
	for (int i = 0; i < 2 * (n + m); i += 2) {
		int x = p[i];
		int y = p[i + 1];
		set0();
		dfs(c[x].first, c[x].second, position[x]);

		if (!used[c[y].first][c[y].second][position[y]]) {
			return false;
		}

		for (int j = 0; j < 2 * (n + m); ++j) {
			if (j == i || j == i + 1) {
				continue;
			}
			int t = p[j];
			if (used[c[t].first][c[t].second][position[t]]) {
				return false;
			}
		}
	}
	return true;
}

void solve(int test) {
	cin >> n >> m;
	for (int i = 0; i < 2 * (n + m); ++i) {
		cin >> p[i];
		--p[i];
	}

	for (int i = 0; i < m; ++i) {
		c[i] = make_pair(0, i);
		position[i] = 0;
	}
	for (int i = m; i < m + n; ++i) {
		c[i] = make_pair(i - m, m - 1);
		position[i] = 1;
	}
	for (int i = m + n; i < m + m + n; ++i) {
		int y = i - m - n;
		y = m - 1 - y;
		c[i] = make_pair(n - 1, y);
		position[i] = 2;
	}
	for (int i = m + m + n; i < m + m + n + n; ++i) {
		int x = i - m - m - n;
		x = n - 1 - x;
		c[i] = make_pair(x, 0);
		position[i] = 3;
	}

	int t = n * m;
	printf("Case #%d:\n", test);
	for (int i = 0; i < (1 << t); ++i) {
		for (int x = 0; x < n; ++x) {
			for (int y = 0; y < m; ++y) {
				int j = x * m + y;
				if (i & (1 << j)) {
					maze[x][y] = '/';
				} else {
					maze[x][y] = '\\';
				}
			}
		}

		if (check()) {
			for (int x = 0; x < n; ++x) {
				for (int y = 0; y < m; ++y) {
					cout << maze[x][y];
				}
				cout << endl;
			}
			return;
		}
	}
	cout << "IMPOSSIBLE" << endl;
}

int main(int argc, char* argv[]) {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	int left_bound, right_bound;

	/*freopen(argv[3], "w", stdout);	
	sscanf(argv[1], "%d", &left_bound);
	sscanf(argv[2], "%d", &right_bound);*/

	int t;
	cin >> t;
	left_bound = 1, right_bound = t;
	for (int i = 1; i <= t; ++i) {
		if (i >= left_bound && i <= right_bound) {
			solve(i);
		}
		cerr << i << ": " << clock() << endl;
	}

	return 0;
}
