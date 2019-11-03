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

const int maxN = 10;
vector < string > s;
vector < int > order;
int n;
bool working[maxN];

bool rec(int m) {
	if (m == n) {
		return true;
	}

	int cnt = 0;
	for (int i = 0; i < n; ++i) {
		if (s[order[m]][i] == '1' && working[i] == 0) {
			++cnt;
			working[i] = 1;
			if (!rec(m + 1)) {
				return false;
			}
			working[i] = 0;
		}
	}
	if (cnt == 0) {
		return false;
	}
	return true;
}

bool check() {
	vector < int > order(n);
	for (int i = 0; i < n; ++i) {
		order[i] = i;
	}
	do {
		::order = order;
		for (int i = 0; i < n; ++i) {
			working[i] = 0;
		}
		if (!rec(0)) {
			return false;
		}
	} while (next_permutation(order.begin(), order.end()));
	return true;
}

void solve(int test) {
	cin >> n;
	s.assign(n, "");
	for (int i = 0; i < n; ++i) {
		cin >> s[i];
	}

	vector < pair < int, int > > positions;
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			if (s[i][j] == '0') {
				positions.push_back(make_pair(i, j));
			}
		}
	}

	int m = positions.size();
	int res = m;
	for (int i = 0; i < (1 << m); ++i) {
		int cnt = 0;
		for (int j = 0; j < m; ++j) {
			if (i & (1 << j)) {
				++cnt;
			}
		}
		if (cnt >= res) {
			continue;
		}

		for (int j = 0; j < m; ++j) {
			if (i & (1 << j)) {
				s[positions[j].first][positions[j].second] = '1';
			}
		}
		if (check()) {
			res = min(res, cnt);
			check();
		}
		for (int j = 0; j < m; ++j) {
			if (i & (1 << j)) {
				s[positions[j].first][positions[j].second] = '0';
			}
		}
	}
	printf("Case #%d: %d\n", test, res);
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
