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

struct Ast {
	double x, y, z;
	double vx, vy, vz;

	Ast() {

	}

	void read() {
		cin >> x >> y >> z >> vx >> vy >> vz;
	}

	double dist(const Ast& other) const {
		return sqrt((x - other.x) * (x - other.x) + (y - other.y) * (y - other.y) + (z - other.z) * (z - other.z));
	}
};

void dfs(const vector<vector<int>>& g, int v, vector<int>& used) {
	if (used[v]) {
		return;
	}
	used[v] = 1;
	for (int i = 0; i < g[v].size(); ++i) {
		if (g[v][i]) {
			dfs(g, i, used);
		}
	}
}

bool solve(const vector<Ast>& a, double d) {
	vector<vector<int> > g(a.size(), vector<int>(a.size(), 0));

	for (int i = 0; i < a.size(); ++i) {
		for (int j = 0; j < a.size(); ++j) {
			if (a[i].dist(a[j]) < d) {
				g[i][j] = 1;
			}
		}
	}

	vector<int> used(a.size());
	dfs(g, 0, used);
	return used[1];
}

void solve(int tcase) {
	int n, s;
	cin >> n >> s;

	cout << "Case #" << tcase << ": ";

	vector<Ast> a(n);

	for (int i = 0; i < n; ++i) {
		a[i].read();
	}

	double l = 0, r = 1e10;

	for (int i = 0; i < 100; ++i) {
		double key = (l + r) / 2;

		if (solve(a, key)) {
			r = key;
		} else {
			l = key;
		}
	}
	printf("%.10lf\n", l);
}

int main() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	int t;
	cin >> t;

	for (int i = 0; i < t; ++i) {
		cerr << i << endl;
		solve(i + 1);
	}

	return 0;
}