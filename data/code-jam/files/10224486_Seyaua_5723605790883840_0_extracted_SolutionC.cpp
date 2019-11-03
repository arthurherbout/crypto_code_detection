#include <iostream>
#include <string>
#include <cstring>
#include <set>
#include <map>
#include <vector>
#include <bitset>
#include <queue>
#include <algorithm>
#include <cstdio>
#pragma comment(linker, "/STACK:256000000")

using namespace std;

bool is_in(int x, int y, int n, int m) {
	return x >= 0 && x < n && y >= 0 && y < m;
}

int getUpper(int x, int y, int n, int m) {
	return x * m + y + 2 * (n + m);
}

int getLower(int x, int y, int n, int m) {
	return x * m + y + n * m + 2 * (n + m);
}

void addEdge(vector<vector<int>>& g, int a, int b) {
	g[a].push_back(b);
	g[b].push_back(a);
}

void dfs(const vector<vector<int>>& g, int v, int val, vector<int>& used) {
	if (used[v]) {
		return;
	}
	used[v] = val;
	for (int i = 0; i < g[v].size(); ++i) {
		dfs(g, g[v][i], val, used);
	}
}

vector<int> fillComponents(const vector<vector<int>>& g) {
	vector<int> used(g.size(), 0);

	int idx = 1;
	for (int i = 0; i < g.size(); ++i) {
		if (!used[i]) {
			dfs(g, i, idx, used);
			++idx;
		}
	}
	return used;
}

vector<string> solve(int mask, int n, int m, const vector<int>& pm) {
	vector<string> res(n, string(m, '/'));
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			int bt = i * m + j;
			if (mask & (1 << bt)) {
				res[i][j] = '\\';
			}
		}
	}

	vector<vector<int> > g(2 * (n + m) + 2 * n * m);

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			if (res[i][j] == '/') {
				int tupper = getUpper(i, j, n, m);
				int tlower = getLower(i, j, n, m);
				if (is_in(i - 1, j, n, m)) {
					addEdge(g, tupper, getLower(i - 1, j, n, m));
				}
				if (is_in(i + 1, j, n, m)) {
					addEdge(g, tlower, getUpper(i + 1, j, n, m));
				}
				if (is_in(i, j - 1, n, m)) {
					if (res[i][j - 1] == '/') {
						addEdge(g, tupper, getLower(i, j - 1, n, m));
					} else {
						addEdge(g, tupper, getUpper(i, j - 1, n, m));
					}
				}
				if (is_in(i, j + 1, n, m)) {
					if (res[i][j + 1] == '/') {
						addEdge(g, tlower, getUpper(i, j + 1, n, m));
					} else {
						addEdge(g, tlower, getLower(i, j + 1, n, m));
					}
				}
			} else {
				int tupper = getUpper(i, j, n, m);
				int tlower = getLower(i, j, n, m);
				if (is_in(i - 1, j, n, m)) {
					addEdge(g, tupper, getLower(i - 1, j, n, m));
				}
				if (is_in(i + 1, j, n, m)) {
					addEdge(g, tlower, getUpper(i + 1, j, n, m));
				}
				if (is_in(i, j - 1, n, m)) {
					if (res[i][j - 1] == '/') {
						addEdge(g, tlower, getLower(i, j - 1, n, m));
					} else {
						addEdge(g, tlower, getUpper(i, j - 1, n, m));
					}
				}
				if (is_in(i, j + 1, n, m)) {
					if (res[i][j + 1] == '/') {
						addEdge(g, tupper, getUpper(i, j + 1, n, m));
					} else {
						addEdge(g, tupper, getLower(i, j + 1, n, m));
					}
				}
			}
		}
	}

	for (int i = 0; i < m; ++i) {
		int tupper = getUpper(0, i, n, m);
		addEdge(g, i, tupper);
	}

	int idx = 0;
	for (int i = m; i < m + n; ++i) {
		int tupper = getUpper(idx, m - 1, n, m);
		int tlower = getLower(idx, m - 1, n, m);
		if (res[idx][m - 1] == '/') {
			addEdge(g, tlower, i);
		} else {
			addEdge(g, tupper, i);
		}
		++idx;
	}

	idx = m - 1;

	for (int i = m + n; i < m + n + m; ++i) {
		int tlower = getLower(n - 1, idx, n, m);
		addEdge(g, i, tlower);
		--idx;
	}

	idx = n - 1;
	for (int i = m + n + m; i < 2 * (n + m); ++i) {
		int tlower = getLower(idx, 0, n, m);
		int tupper = getUpper(idx, 0, n, m);
		if (res[idx][0] == '/') {
			addEdge(g, i, tupper);
		} else {
			addEdge(g, i, tlower);
		}
		--idx;
	}

	vector<int> cmp = fillComponents(g);


	vector<string> emp;
	for (int i = 0; i < 2 * (n + m); ++i) {
		if (cmp[pm[i]] != cmp[i] && pm[i] != i) {
			return emp;
		}
	}
	return res;
}

void solve(int tcase) {
	cout << "Case #" << tcase << ":" << endl;
	int n, m;
	cin >> n >> m;
	vector<int> pr(2 * (n + m));

	for (int i = 0; i < pr.size(); ++i) {
		cin >> pr[i];
		--pr[i];
	}

	vector<int> pm(2 * (n + m));
	for (int i = 0; i < pr.size(); i += 2) {
		pm[pr[i]] = pr[i + 1];
		pm[pr[i + 1]] = pr[i];
	}

	int sz = n * m;

	for (int mask = 0; mask < (1 << sz); ++mask) {
		vector<string> res = solve(mask, n, m, pm);

		if (res.size() != 0) {
			for (int i = 0; i < res.size(); ++i) {
				cout << res[i] << endl;
			}
			return;
		}
	}
	cout << "IMPOSSIBLE" << endl;
}

int main() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	int t;
	scanf("%d", &t);

	for (int i = 1; i <= t; ++i) {
		solve(i);
		cerr << i << endl;
	}

	return 0;
}