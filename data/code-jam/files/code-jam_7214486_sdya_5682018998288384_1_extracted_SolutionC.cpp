//#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <cstdio>
#include <vector>
#include <cstdio>
#include <algorithm>
#include <string>
#include <set>
#include <map>
#include <queue>
#include <bitset>
#include <cmath>
#include <ctime>
#pragma comment (linker, "/STACK:256000000")

using namespace std;

const int maxN = 11000;
const long long P = 137LL;

string color[maxN];
int n;
vector < int > g[maxN];
bool used[maxN];
int s[maxN];

long long h[maxN];

void read_data(int test) {
cin >> n;
	for (int i = 0; i < n; ++i) {
		cin >> color[i];
		g[i].clear();
	}

	for (int i = 1; i < n; ++i) {
		int u, v;
		cin >> u >> v;
		--u;
		--v;
		g[u].push_back(v);
		g[v].push_back(u);
	}
}

void dfs(int v, long long Q = 0) {
	used[v] = true;
	s[v] = 1;

	vector < long long > children;
	for (int i = 0; i < g[v].size(); ++i) {
		if (used[g[v][i]]) {
			continue;
		}
		dfs(g[v][i]);
		s[v] += s[g[v][i]];
		children.push_back(h[g[v][i]]);
	}

	sort(children.begin(), children.end());
	long long hash = 0;
	hash = hash * P + s[v] + 1;
	if (Q != 0) {
		hash %= Q;
	}
	hash = hash * P + children.size() + 1;
	if (Q != 0) {
		hash %= Q;
	}
	hash = hash * P + (color[v][0]);
	if (Q != 0) {
		hash %= Q;
	}

	for (int i = 0; i < children.size(); ++i) {
		hash = hash * P + children[i];
		if (Q != 0) {
			hash %= Q;
		}
	}
	h[v] = hash;
}

bool isPossible(int v, int u) {
	int pr = v;
	int current = u;
	while (true) {
		if (g[current].size() == 1) {
			return true;
		}
		if (g[current].size() == 2) {
			if (g[current][0] != pr) {
				pr = current;
				current = g[current][0];
			}
			else {
				pr = current;
				current = g[current][1];
			}
			continue;
		}

		break;
	}

	v = current;
	vector < pair < long long, int > > hashes;
	for (int i = 0; i < g[v].size(); ++i) {
		if (g[v][i] != pr) {
			hashes.push_back(make_pair(h[g[v][i]], g[v][i]));
		}
	}

	sort(hashes.begin(), hashes.end());

	vector < pair < long long, int > > upd;
	for (int i = 0; i < hashes.size(); ++i) {
		if (upd.size() > 0 && upd.back().first == hashes[i].first) {
			upd.pop_back();
			continue;
		}
		upd.push_back(hashes[i]);
	}

	if (upd.size() > 1) {
		return false;
	}

	for (int i = 0; i < upd.size(); ++i) {
		if (!isPossible(v, upd[i].second)) {
			return false;
		}
	}
	return true;
}

bool check(int v) {
	memset(used, 0, sizeof(used));
	memset(s, 0, sizeof(s));

	dfs(v);
	vector < pair < long long, int > > hashes;
	for (int i = 0; i < g[v].size(); ++i) {
		hashes.push_back(make_pair(h[g[v][i]], g[v][i]));
	}

	sort(hashes.begin(), hashes.end());

	vector < pair < long long, int > > upd;
	for (int i = 0; i < hashes.size(); ++i) {
		if (upd.size() > 0 && upd.back().first == hashes[i].first) {
			upd.pop_back();
			continue;
		}
		upd.push_back(hashes[i]);
	}

	if (upd.size() > 2) {
		return false;
	}

	for (int i = 0; i < upd.size(); ++i) {
		if (!isPossible(v, upd[i].second)) {
			return false;
		}
	}
	return true;
}

bool check(int u, int v) {
	if (color[u] != color[v]) {
		return false;
	}

	memset(used, 0, sizeof(used));
	memset(s, 0, sizeof(s));
	dfs(u);

	long long A = h[v];

	memset(used, 0, sizeof(used));
	memset(s, 0, sizeof(s));
	dfs(v);

	long long B = h[u];
	return A == B;
}

void solve2(int test) {
	if (test == 11) {
		bool bad = false;
	}
	cin >> n;
	for (int i = 0; i < n; ++i) {
		cin >> color[i];
		g[i].clear();
	}

	for (int i = 1; i < n; ++i) {
		int u, v;
		cin >> u >> v;
		--u;
		--v;
		g[u].push_back(v);
		g[v].push_back(u);
	}

	for (int i = 0; i < n; ++i) {
		if (check(i)) {
			printf("Case #%d: SYMMETRIC\n", test);
			return;
		}
	}

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < g[i].size(); ++j) {
			if (g[i][j] > i && check(i, g[i][j])) {
				printf("Case #%d: SYMMETRIC\n", test);
				return;
			}
		}
	}

	printf("Case #%d: NOT SYMMETRIC\n", test);
}

int main(int argc, char* argv[]) {
	freopen("input.txt", "r", stdin);
	//freopen("output.txt", "w", stdout);

	//freopen("input.txt", "r", stdin);
	//freopen(argv[3], "w", stdout);

	int left_bound, right_bound;
	//sscanf(argv[1], "%d", &left_bound);
	//sscanf(argv[2], "%d", &right_bound);

	int tests;
	scanf("%d", &tests);
	left_bound = 1;
	right_bound = tests;
	for (int i = 1; i <= tests; ++i) {
		if (i >= left_bound && i <= right_bound) {
			solve2(i);
			cerr << i << ": " << clock() << endl;
		}
		else {
			read_data(i);
		}
	}
	return 0;
}
