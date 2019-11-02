#include <iostream>
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

void read_data(int test) {
}

const int maxM = 1100;
const int maxN = 110;
int m, n;
string s[maxM];
int best = 0, ways = 0;

vector < int > ids[maxN];

void update() {
	for (int i = 0; i < n; ++i) {
		if (ids[i].size() == 0) {
			return ;
		}
	}
	int total = 0;
	for (int i = 0; i < n; ++i) {
		set < string > S;
		for (int j = 0; j < ids[i].size(); ++j) {
			string cur = s[ids[i][j]];
			for (int k = 0; k <= cur.size(); ++k) {
				S.insert(cur.substr(0, k));
			}
		}
		total += S.size();
	}
	if (total > best) {
		best = total;
		ways = 1;
	} else if (total == best) {
		++ways;
	}
}

void rec(int have) {
	if (have == m) {
		update();
		return ;
	}

	for (int i = 0; i < n; ++i) {
		ids[i].push_back(have);
		rec(have + 1);
		ids[i].pop_back();
	}
}

void solve(int test) {
	cin >> m >> n;
	for (int i = 0; i < m; ++i) {
		cin >> s[i];
	}
	for (int i = 0; i < n; ++i) {
		ids[i].clear();
	}
	best = 0, ways = 0;
	rec(0);
	printf("Case #%d: %d %d\n", test, best, ways);
}

int main(int argc, char* argv[]) {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
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
			solve(i);
			cerr << i << ": " << clock() << endl;
		} else {
			read_data(i);
		}
	}
	return 0;
}