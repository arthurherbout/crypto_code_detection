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

const int maxN = 40;
int n, m, moves;
string s[maxN];
vector < pair < int, int > > a, b;

int used[maxN][maxN];

int dx[] = {-1, 0, 0, 1};
int dy[] = { 0,-1, 1, 0};

bool isValid(int x, int y) {
	return 0 <= x && x < n && 0 <= y && y < m && s[x][y] != '#';
}

void bfs(int u, int v) {
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			used[i][j] = -1;
		}
	}
	used[u][v] = 0;
	queue < pair < int, int > > q;
	q.push(make_pair(u, v));

	while (!q.empty()) {
		pair < int, int > p = q.front();
		q.pop();
		int u = p.first;
		int v = p.second;
		if (used[u][v] == moves) {
			break;
		}

		bool attacked = false;
		for (int i = 0; i < 4 && !attacked; ++i) {
			for (int j = 1; j <= max(n, m) && !attacked; ++j) {
				int nx = u + dx[i] * j;
				int ny = v + dy[i] * j;
				if (!isValid(nx, ny)) {
					break;
				}

				if (s[nx][ny] == 'T') {
					attacked = true;
				}
			}
		}
		if (!attacked) {
			for (int i = 0; i < 4; ++i) {
				int x = u + dx[i];
				int y = v + dy[i];
				if (isValid(x, y) && s[x][y] != 'T' && used[x][y] == -1) {
					used[x][y] = used[u][v] + 1;
					q.push(make_pair(x, y));
				}
			}
		}
	}
}

bool canKill(int x, int y) {
	memset(used, 0, sizeof(used));
	bfs(a[x].first, a[x].second);

	int res = 1000000000;
	for (int i = 0; i < 4; ++i) {
		for (int j = 1; j <= max(n, m); ++j) {
			int nx = b[y].first + dx[i] * j;
			int ny = b[y].second + dy[i] * j;
			if (!isValid(nx, ny)) {
				break;
			}
			if (used[nx][ny] != -1) {
				res = min(res, used[nx][ny]);
			}
		}
	}
	return (res <= moves);
}

int useds[maxN];
int usedt[maxN];

vector < pair < int, int > > ans, curans;

void rec(int kills, int &res, int &iters) {
	++iters;
	if (iters > 5000) {
		return;
	}
	if (res == b.size()) {
		return;
	}
	if (kills > res) {
		ans = curans;
		res = kills;
		cerr << kills << endl;
	}
	for (int i = 0; i < a.size(); ++i) {
		for (int j = 0; j < b.size(); ++j) {
			if (!useds[i] && !usedt[j]) {
				if (canKill(i, j)) {
					s[b[j].first][b[j].second] = '.';
					useds[i] = true;
					usedt[j] = true;
					curans.push_back(make_pair(i + 1, j + 1));
					rec(kills + 1, res, iters);
					s[b[j].first][b[j].second] = 'T';
					curans.pop_back();
					useds[i] = false;
					usedt[j] = false;
				}
			}
		}
	}
}

void solve(int test) {
	cin >> m >> n >> moves;
	for (int i = 0; i < n; ++i) {
		cin >> s[i];
	}
	a.clear();
	b.clear();
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			if (s[i][j] == 'S') {
				a.push_back(make_pair(i, j));
			}
			if (s[i][j] == 'T') {
				b.push_back(make_pair(i, j));
			}
		}
	}

	memset(useds, 0, sizeof(useds));
	memset(usedt, 0, sizeof(usedt));

	int res = 0;
	int iters = 0;
	ans.clear();
	curans.clear();
	rec(0, res, iters);
	printf("Case #%d: %d\n", test, res);
	for (int i = 0; i < ans.size(); ++i) {
		printf("%d %d\n", ans[i].first, ans[i].second);
	}
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
