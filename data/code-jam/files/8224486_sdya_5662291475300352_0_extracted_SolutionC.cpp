#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include <bitset>
#include <queue>
#include <cstring>
#include <string>
#include <cmath>
#include <ctime>
#pragma comment (linker, "/STACK:256000000")

using namespace std;

vector < pair < int, int > > s;
int n;

long long upper(double t) {
	long long m = t / 360.0;
	for (long long i = m - 1; ; ++i) {
		if ((double)(i * 360LL) >= t - 1e-9) {
			return i;
		}
	}
}

long long lower(double t) {
	long long m = t / 360.0;
	for (long long i = m + 1; ; --i) {
		if ((double)(i * 360LL) <= t + 1e-9) {
			return i;
		}
	}
}

long long calculate(double h, int start, int u) {
	if (fabs((double)(u) - h) < 1e-12) {
		return 0LL;
	}
	double x = 360.0 / h;
	double m = 360.0 / (double)(u);
	if (x < m) {
		double L = start;
		double R = h * (m - x) + (double)(start);

		long long l = upper(L);
		long long r = lower(R);
		if (l <= r) {
			return r - l + 1;
		}
	} else {
		double L = (double)(start) - h * (x - m);
		double R = start;

		long long l = upper(L);
		long long r = lower(R);
		if (l <= r) {
			return r - l + 1;
		}
	}
	return 0;
}

long long calculate(double t) {
	if (t <= 1e-9) {
		return 1000000000000000000LL;
	}
	long long res = 0;
	for (int i = 0; i < s.size(); ++i) {
		res += calculate(t, s[i].first, s[i].second);
	}
	return res;
}

void solve(int test) {
	s.clear();
	cin >> n;

	vector < double > candidates;
	for (int i = 0; i < n; ++i) {
		int d, h, m;
		cin >> d >> h >> m;
		for (int j = 0; j < h; ++j) {
			s.push_back(make_pair(d, m + j));
			candidates.push_back(m + j);
			candidates.push_back((360.0 - (double)(d)) / 360.0 * (double)(m + j));
			candidates.push_back((720.0 - (double)(d)) / 360.0 * (double)(m + j));
		}
	}

	candidates.push_back(0);
	candidates.push_back(2000000000);
	sort(candidates.begin(), candidates.end());
	long long res = 1000000000000000000LL;
	for (int i = 0; i < candidates.size(); ++i) {
		double d = candidates[i];
		res = min(res, calculate(d));

		for (int dx = -10; dx <= 10; ++dx) {
			res = min(res, calculate(d + dx * 1e-5));
		}
		for (int dx = 1; dx <= 20000; ++dx) {
			res = min(res, calculate(d / 10000.0 * (double)(dx)));
		}
		res = min(res, calculate(d + 1e-6));
		res = min(res, calculate(d - 1e-6));

		if (i > 0) {
			res = min(res, calculate(((double)(candidates[i - 1]) + d) / 2.0));
		}
	}

	/*for (int i = 1; i < candidates.size(); ++i) {
		double l = candidates[i - 1];
		double r = candidates[i];

		double d = 1000000.0;
		for (int j = 0; j <= 1000000; ++j) {
			double x = l + (double)(r - l) / d;
			res = min(res, calculate(x));
		}
	}*/
	printf("Case #%d: ", test);
	cout << res << endl;
}

int main() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	int tests;
	scanf("%d", &tests);
	for (int i = 0; i < tests; ++i) {
		solve(i + 1);
		cerr << (i + 1) << endl;
	}
	cerr << "Time: " << clock() << endl;
	return 0;
}