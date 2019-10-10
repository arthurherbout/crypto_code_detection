#include <iostream>
#include <string>
#include <cstring>
#include <cstdio>
#include <set>
#include <map>
#include <vector>
#include <algorithm>
#include <queue>
#include <bitset>
#include <cmath>

using namespace std;

const int maxN = 2100;

int s[maxN];
int m[maxN];
int D;

vector <int> g[maxN];
int cnt;

void dfs(int v, int mx) {
	if (s[v] > mx || s[v] < mx - D) {
		return;
	}
	++cnt;
	for (int i = 0; i < g[v].size(); ++i) {
		dfs(g[v][i], mx);
	}
}

void solve(int tcase) {
	printf("Case #%d: ", tcase);
	int n, d;
	cin >> n >> d;
	D = d;
	int s0, as, cs, rs;
	int m0, am, cm, rm;

	for (int i = 0; i < n; ++i) {
		g[i].clear();
	}

	cin >> s0 >> as >> cs >> rs;
	cin >> m0 >> am >> cm >> rm;

	s[0] = s0;
	m[0] = m0;

	for (int i = 0; i + 1 < n; ++i) {
		s[i + 1] = (s[i] * as + cs) % rs;
		m[i + 1] = (m[i] * am + cm) % rm;
	}

	for (int i = 1; i < n; ++i) {
		m[i] %= i;
		g[m[i]].push_back(i);
	}
	m[0] = 0;

	int ans = 0;
	for (int i = 0; i < n; ++i) {
		cnt = 0;
		dfs(0, s[i]);
		ans = max(ans, cnt);
	}
	cout << ans << endl;
}

int main() {
	freopen("input.txt", "r", stdin);
	freopen("GCJ2015R3A.txt", "w", stdout);

	int t;
	scanf("%d", &t);

	for (int i = 1; i <= t; ++i) {
		solve(i);
	}

	return 0;
}

