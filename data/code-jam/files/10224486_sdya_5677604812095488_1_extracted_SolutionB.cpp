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

const int maxN = 1010;
double d[maxN][maxN];

double calc(vector < double > p) {
	int n = p.size();
	for (int i = 0; i <= n; ++i) {
		for (int j = 0; j <= n; ++j) {
			d[i][j] = 0.0;
		}
	}
	d[0][0] = 1.0;
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j <= n; ++j) {
			d[i + 1][j] += d[i][j] * (1.0 - p[i]);
			d[i + 1][j + 1] += d[i][j] * p[i];
		}
	}
	return d[n][n / 2];
}

void solve2(int test) {
	int n, k;
	cin >> n >> k;
	vector < double > p(n);
	for (int i = 0; i < n; ++i) {
		cin >> p[i];
	}
	sort(p.begin(), p.end());

	double res = 0.0;
	for (int i = 0; i <= k; ++i) {
		vector < double > q;
		int L = i;
		int R = k - i;
		for (int j = 0; j < L; ++j) {
			q.push_back(p[j]);
		}
		for (int j = n - 1; j >= n - R; --j) {
			q.push_back(p[j]);
		}
		res = max(res, calc(q));
	}
	printf("Case #%d: %.10lf\n", test, res);
}

void solve(int test) {
	int n, k;
	cin >> n >> k;
	vector < double > p(n);
	for (int i = 0; i < n; ++i) {
		cin >> p[i];
	}

	double res = 0.0;
	for (int i = 0; i < (1 << n); ++i) {
		vector < double > q;
		for (int j = 0; j < n; ++j) {
			if (i & (1 << j)) {
				q.push_back(p[j]);
			}
		}
		if (q.size() != k) {
			continue;
		}
		res = max(res, calc(q));
	}
	printf("Case #%d: %.10lf\n", test, res);
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
			solve2(i);
		}
		cerr << i << ": " << clock() << endl;
	}

	return 0;
}
