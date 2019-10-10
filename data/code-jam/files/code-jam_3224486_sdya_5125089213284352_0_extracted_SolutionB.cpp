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

const int maxN = 110;
const int maxL = 12000;

vector < int > g[maxN];
int n;
string s;

vector < string > generateStrings(int v) {
	vector < vector < string > > parts;
	for (int i = 0; i < g[v].size(); ++i) {
		parts.push_back(generateStrings(g[v][i]));
	}

	vector < string > res;
	for (int i = 0; i < maxL; ++i) {
		vector < string > items;
		for (int j = 0; j < parts.size(); ++j) {
			items.push_back(parts[j][rand() % parts[j].size()]);
		}

		string current = "";
		vector < int > ids(items.size(), 0);

		vector < int > order;
		for (int j = 0; j < items.size(); ++j) {
			for (int k = 0; k < items[j].size(); ++k) {
				order.push_back(j);
			}
		}
		random_shuffle(order.begin(), order.end());

		for (int j = 0; j < order.size(); ++j) {
			current += items[order[j]][ids[order[j]]++];
		}
		if (v == n) {
			res.push_back(current);
		} else {
			res.push_back(string(1, s[v]) + current);
		}
	}
	return res;
}

void solve(int test) {
	cin >> n;
	vector < int > p(n);
	for (int i = 0; i < n; ++i) {
		cin >> p[i];
		--p[i];
	}

	for (int i = 0; i <= n; ++i) {
		g[i].clear();
	}

	for (int i = 0; i < n; ++i) {
		if (p[i] == -1) {
			g[n].push_back(i);
		} else {
			g[p[i]].push_back(i);
		}
	}

	cin >> s;

	vector < string > items = generateStrings(n);

	int m;
	cin >> m;
	vector < string > t(m);
	for (int i = 0; i < m; ++i) {
		cin >> t[i];
	}

	printf("Case #%d:", test);
	for (int i = 0; i < m; ++i) {
		int cnt = 0;
		for (int j = 0; j < items.size(); ++j) {
			if (items[j].find(t[i]) != string::npos) {
				++cnt;
			}
		}
		printf(" %.6lf", (double)(cnt) / (double)(items.size()));
	}
	printf("\n");

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
		} else {
			readData();
		}
		cerr << i << ": " << clock() << endl;
	}

	return 0;
}
