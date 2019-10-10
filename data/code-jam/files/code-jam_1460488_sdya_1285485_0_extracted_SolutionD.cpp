#include <iostream>
#include <string>
#include <cstring>
#include <cstdio>
#include <algorithm>
#include <set>
#include <vector>
#include <map>
#include <cmath>

using namespace std;

const int maxN = 60;

int n, m, d, sx, sy;
char s[maxN][maxN];
char grid[maxN][maxN];

bool comp(pair < int, int > A, pair < int, int > B) {
	return A.first * A.first + A.second * A.second < B.first * B.first + B.second * B.second;
}

pair < int, int > produce(int i, int j) {
	int addx = i * n, addy = j * m;
	if (i % 2 == 0) {
		addx += sx;
	} else {
		addx += n - 1 - sx;
	}

	if (j % 2 == 0) {
		addy += sy;
	} else {
		addy += m - 1 - sy;
	}
	return make_pair(addx, addy);
}

double dist(pair < int, int > A, pair < int, int > B) {
	int dx = A.first - B.first;
	int dy = A.second - B.second;
	return sqrt((double)(dx * dx + dy * dy));
}

void solve(int test) {
	scanf("%d%d%d\n", &n, &m, &d);
	for (int i = 0; i < n; ++i) {
		gets(s[i]);
	}

	for (int i = 1; i + 1 < n; ++i)
		for (int j = 1; j + 1 < m; ++j)
			grid[i - 1][j - 1] = s[i][j];
	n -= 2;
	m -= 2;

	for (int i = 0; i < n; ++i)
		for (int j = 0; j < m; ++j)
			if (grid[i][j] == 'X') {
				sx = i;
				sy = j;
			}

	int res = 0;
	set < pair < int, int > > used;
	used.insert(make_pair(0, 0));

	vector < pair < int, int > > order;
	for (int i = -50; i <= 50; ++i)
		for (int j = -50; j <= 50; ++j) {
			order.push_back(make_pair(i, j));
		}
	sort(order.begin(), order.end(), comp);

	for (int index = 0; index < order.size(); ++index) {
		int i = order[index].first, j = order[index].second;
		if (used.count(make_pair(i, j)) != 0) continue;

		int addx = i * n, addy = j * m;
		if (i % 2 == 0) {
			addx += sx;
		} else {
			addx += n - 1 - sx;
		}

		if (j % 2 == 0) {
			addy += sy;
		} else {
			addy += m - 1 - sy;
		}

		if ((addx - sx) * (addx - sx) + (addy - sy) * (addy - sy) <= d * d) {
			++res;
			for (int k = index + 1; k < order.size(); ++k) {
				pair < int, int > A = produce(0, 0);
				pair < int, int > B = produce(i, j);
				pair < int, int > C = produce(order[k].first, order[k].second);
				double d1 = dist(A, B);
				double d2 = dist(B, C);
				double d3 = dist(A, C);
				if (fabs(d3 - d1 - d2) < 1e-9) {
					used.insert(order[k]);
				}
			}
		}
	}
	printf("Case #%d: %d\n", test, res);
}

int main() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	int tests;
	scanf("%d", &tests);
	for (int i = 1; i <= tests; ++i) {
		solve(i);
		cerr << i << endl;
	}
	return 0;
}