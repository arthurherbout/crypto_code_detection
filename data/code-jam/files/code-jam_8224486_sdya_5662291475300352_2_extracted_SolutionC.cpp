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

const int maxN = 2000000;
long long t[maxN * 8];
long long add[maxN * 8];

void update(int i, int l, int r, int cl, int cr, int delta) {
	if (cl > cr) {
		return ;
	}
	if (l == cl && r == cr) {
		t[i] += delta;
		add[i] += delta;
		return ;
	}
	if (cl > (l + r) / 2) {
		update(2 * i + 1, (l + r) / 2 + 1, r, cl, cr, delta);
	} else if (cr <= (l + r) / 2) {
		update(2 * i, l, (l + r) / 2, cl, cr, delta);
	} else {
		update(2 * i, l, (l + r) / 2, cl, (l + r) / 2, delta);
		update(2 * i + 1, (l + r) / 2 + 1, r, (l + r) / 2 + 1, cr, delta);
	}
	t[i] = max(t[2 * i], t[2 * i + 1]) + add[i];
}

int cleverSolve() {
	vector < pair < long long, long long > > a;
	vector < long long > b;
	for (int i = 0; i < s.size(); ++i) {
		long long l = (360LL - (long long)(s[i].first)) * (long long)(s[i].second);
		long long r = (720LL - (long long)(s[i].first)) * (long long)(s[i].second);
		l *= 3;
		r *= 3;
		a.push_back(make_pair(l + 1, r - 1));
		b.push_back(l + 1);
		b.push_back(r - 1);
	}
	sort(b.begin(), b.end());
	b.resize(unique(b.begin(), b.end()) - b.begin());

	int m = b.size();
	for (int i = 0; i < maxN * 8; ++i) {
		t[i] = add[i] = 0;
	}
	for (int i = 0; i < a.size(); ++i) {
		int l = lower_bound(b.begin(), b.end(), a[i].first) - b.begin();
		int r = lower_bound(b.begin(), b.end(), a[i].second) - b.begin();
		update(1, 0, m - 1, l, r, +1);
	}

	return (int)(s.size()) - t[1];
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

	//printf("Case #%d: %d\n", test, cleverSolve());
	//return ;

	long long res = 1000000000000000000LL;
	double l = 0.0, r = 2000000000.0;
	for (int i = 0; i < 200; ++i) {
		double l1 = (2.0 * l + r) / 3.0;
		double l2 = (l + 2.0 * r) / 3.0;
		

		long long a = calculate(l1);
		long long b = calculate(l2);

		if (a < b) {
			r = l2;
		} else {
			l = l1;
		}
		res = min(res, min(a, b));
	}

	if (s.size() <= 50) {
		sort(candidates.begin(), candidates.end());
		for (int i = 0; i < candidates.size(); ++i) {
			double d = candidates[i];
			res = min(res, calculate(d));

			for (int dx = 1; dx <= 2000; ++dx) {
				res = min(res, calculate(d / 1000.0 * (double)(dx)));
			}
			res = min(res, calculate(d + 1e-6));
			res = min(res, calculate(d - 1e-6));

			if (i > 0) {
				res = min(res, calculate(((double)(candidates[i - 1]) + d) / 2.0));
			}
		}
	}
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