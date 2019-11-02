#pragma comment(linker, "/STACK:100000000")
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <cmath>
#include <ctime>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <queue>
#include <algorithm>
#include <iostream>
using namespace std;
#define int64 long long
#define ldb long double
#define pb push_back
#define mp make_pair
#define fs first
#define sc second
#define sz(a) ((int) (a).size())
#define eprintf(...) fprintf(stderr, __VA_ARGS__)
#define taskname "task_d"
const ldb pi = acos(-1.0);
const int N = 100;
const int inf = (int) 1e9;
bool used[N][N];
int a[N], cost[N], n, t, u, v;
vector<int> adj[N];

int get(int si, int sj, int i, int j, bool cant) {
	int oa, res = -inf;
	for (int k = 0; k < sz(adj[i]); ++k) {
		int v = adj[i][k];
		if (used[i][v]) continue;
		used[i][v] = used[v][i] = true, oa = a[v], a[v] = 0;
		res = max(res, oa - get(sj, si, j, v, false));
		used[i][v] = used[v][i] = false, a[v] = oa;
	}
	if (res == -inf) return (cant ? 0 : -get(sj, si, j, i, true));
	else return res;
}

int solve(int i, int j) {
	for (int k = 0; k < n; ++k) {
		a[k] = cost[k];
		for (int l = 0; l < n; ++l)
			used[k][l] = false;
	}
	int res = 0;
	res += a[i], a[i] = 0;
	res -= a[j], a[j] = 0;
	return get(i, j, i, j, false) + res;
}

int main() {
	assert(freopen(taskname".in", "r", stdin));
	assert(freopen(taskname".out", "w", stdout));
	scanf("%d", &t);
	for (int it = 0; it < t; ++it) {
		scanf("%d", &n);
		for (int i = 0; i < n; ++i)
			scanf("%d", &cost[i]), adj[i].clear();
		for (int i = 0; i + 1 < n; ++i)
			scanf("%d", &u), --u, adj[u].pb(i), adj[i].pb(u);
		int ans = -inf;
		for (int i = 0; i < n; ++i) {
			int res = inf;
			for (int j = 0; j < n; ++j)
				res = min(res, solve(i, j));
			ans = max(ans, res);
		}
		printf("Case #%d: %d\n", it + 1, ans);
	}
	return 0;
}

