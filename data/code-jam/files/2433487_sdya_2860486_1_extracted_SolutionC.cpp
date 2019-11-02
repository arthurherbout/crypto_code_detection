#include <iostream>
#include <string>
#include <cstring>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include <bitset>
#include <queue>
#include <cmath>
#pragma comment (linker, "/STACK:256000000")

using namespace std;

const int maxN = 2100;
const int maxM = 2100;
const int maxP = 2100;
const long long inf = 1000000000000000000LL; // CHANGE THAT FOR LARGE!!!
int n, m, p;

vector < pair < int, int > > g_down[maxN], g_up[maxN];
vector < int > id[maxN];

vector < pair < int, int > > g_inv_down[maxN], g_inv_up[maxN];
vector < int > id_inv[maxN];

int u[maxM], v[maxM], a[maxM], b[maxM];
int edges[maxP];

//int d1[maxN], d2[maxN];

/*int calculate(int mask) {
	for (int i = 1; i <= n; ++i) {
		d1[i] = inf;
	}
	d1[1] = 0;

	set < pair < int, int > > S;
	S.insert(make_pair(d1[1], 1));
	while (!S.empty()) {
		int v = S.begin()->second;
		S.erase(S.begin());

		for (int i = 0; i < id[v].size(); ++i) {
			int weight = 0;
			if (mask & (1 << id[v][i])) {
				weight = g_down[v][i].second;
			} else {
				weight = g_up[v][i].second;
			}

			int to = g_down[v][i].first;

			if (d1[to] > d1[v] + weight) {
				S.erase(make_pair(d1[to], to));
				d1[to] = d1[v] + weight;
				S.insert(make_pair(d1[to], to));
			}
		}
	}

	for (int i = 1; i <= n; ++i) {
		d2[i] = inf;
	}
	d2[2] = 0;

	S.clear();
	S.insert(make_pair(d2[2], 2));
	while (!S.empty()) {
		int v = S.begin()->second;
		S.erase(S.begin());

		for (int i = 0; i < id_inv[v].size(); ++i) {
			int weight = 0;
			if (mask & (1 << id_inv[v][i])) {
				weight = g_inv_down[v][i].second;
			} else {
				weight = g_inv_up[v][i].second;
			}

			int to = g_inv_down[v][i].first;

			if (d2[to] > d2[v] + weight) {
				S.erase(make_pair(d2[to], to));
				d2[to] = d2[v] + weight;
				S.insert(make_pair(d2[to], to));
			}
		}
	}

	int current = 1;
	int res = 0;
	for (int i = 0; i < p; ++i) {
		bool nice = false;
		for (int j = 0; j < g_down[current].size() && !nice; ++j) {
			int weight = 0;
			if (mask & (1 << id[current][j])) {
				weight = g_down[current][j].second;
			} else {
				weight = g_up[current][j].second;
			}

			int to = g_down[current][j].first;
			if (d1[current] + weight + d2[to] == d1[2] && id[current][j] == edges[i]) {
				nice = true;
				current = to;
				break;
			}
		}

		if (nice) {
			++res;
		} else {
			break;
		}
	}
	return res;
}*/

int need_min[maxM];
long long d[maxN];
int parent[maxN];

bool check(int parts, int mode) {
	for (int i = 0; i < m; ++i) {
		need_min[i] = false;
	}

	for (int i = 0; i < parts; ++i) {
		need_min[edges[i]] = true;
	}

	for (int i = 1; i <= n; ++i) {
		d[i] = inf;
		parent[i] = -1;
	}

	int current = 1;
	if (parts > 0) {
		current = v[edges[parts - 1]];
	}

	d[current] = 0;
	set < pair < long long, int > > S;
	S.insert(make_pair(d[current], current));

	while (!S.empty()) {
		int v = S.begin()->second;
		S.erase(S.begin());

		for (int i = 0; i < g_down[v].size(); ++i) {
			long long weight = (mode ? g_down[v][i].second : g_up[v][i].second);
			int to = g_down[v][i].first;

			if (need_min[id[v][i]]) {
				continue;
			}

			if (d[to] > d[v] + weight) {
				S.erase(make_pair(d[to], to));
				d[to] = d[v] + weight;
				parent[to] = id[v][i];
				S.insert(make_pair(d[to], to));
			}
		}
	}

	if (d[2] == inf) {
		return false;
	}

	long long current_distance = d[2];
	for (int i = 0; i < parts; ++i) {
		current_distance += (long long)(a[edges[i]]);
	}

	current = 2;
	while (parent[current] != -1) {
		need_min[parent[current]] = mode;
		current = u[parent[current]];
	}

	for (int i = 1; i <= n; ++i) {
		d[i] = inf;
	}

	d[1] = 0;
	S.clear();
	S.insert(make_pair(d[1], 1));
	while (!S.empty()) {
		int v = S.begin()->second;
		S.erase(S.begin());

		for (int i = 0; i < g_down[v].size(); ++i) {
			long long weight = (need_min[id[v][i]] ? g_down[v][i].second : g_up[v][i].second);
			int to = g_down[v][i].first;

			if (d[to] > d[v] + weight) {
				S.erase(make_pair(d[to], to));
				d[to] = d[v] + weight;
				S.insert(make_pair(d[to], to));
			}
		}
	}

	return d[2] == current_distance;
}

void solve(int test) {
	cin >> n >> m >> p;
	for (int i = 1; i <= n; ++i) {
		g_down[i].clear();
		g_up[i].clear();
		id[i].clear();

		g_inv_down[i].clear();
		g_inv_up[i].clear();
		id_inv[i].clear();
	}
	for (int i = 0; i < m; ++i) {
		cin >> u[i] >> v[i] >> a[i] >> b[i];
		g_down[u[i]].push_back(make_pair(v[i], a[i]));
		g_up[u[i]].push_back(make_pair(v[i], b[i]));
		id[u[i]].push_back(i);

		g_inv_down[v[i]].push_back(make_pair(u[i], a[i]));
		g_inv_up[v[i]].push_back(make_pair(u[i], b[i]));
		id_inv[v[i]].push_back(i);
	}

	for (int i = 0; i < p; ++i) {
		cin >> edges[i];
		--edges[i];
	}

	int left_bound = 0, right_bound = p;
	while (right_bound - left_bound > 1) {
		int middle = (left_bound + right_bound) / 2;
		if (check(middle, 0) || check(middle, 1)) {
			left_bound = middle;
		} else {
			right_bound = middle;
		}
	}

	int res = 0;
	if (check(right_bound, 0) || check(right_bound, 1)) {
		res = right_bound;
	} else if (check(left_bound, 0) || check(left_bound, 1)) {
		res = left_bound;
	}

	/*int res = 0;
	for (int mask = 0; mask < (1 << m); ++mask) {
		res = max(res, calculate(mask));
	}*/

	if (res == p) {
		printf("Case #%d: Looks Good To Me\n", test);
	} else {
		printf("Case #%d: %d\n", test, edges[res] + 1);
	}
}

int main() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	int tests;
	scanf("%d\n", &tests);
	for (int i = 1; i <= tests; ++i) {
		solve(i);
		cerr << i << endl;
	}

	return 0;
}