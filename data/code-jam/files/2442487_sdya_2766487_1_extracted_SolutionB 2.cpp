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

bool checkAlways(long long n, long long p, long long k) {
	long long m = (1LL << n);
	if (p <= m / 2) {
		return k == 0;
	} else {
		if (k == 0) {
			return true;
		}

		if (k % 2LL == 1LL) {
			return checkAlways(n - 1, p - m / 2, min(m / 2 - 1, k / 2));
		} else {
			return checkAlways(n - 1, p - m / 2, min(m / 2 - 1, (k - 1) / 2));
		}
	}
}

bool checkOnce(long long n, long long p, long long k) {
	long long m = (1LL << n);
	if (n == 1) {
		if (k == 0) {
			return true;
		}

		return p == 2;
	}

	if (k == m - 1) {
		return p == m;
	}

	if (p <= m / 2) {
		if (k == m - 1) {
			return false;
		}

		if (k % 2LL == 0LL) {
			return checkOnce(n - 1, p, min(m / 2 - 1, k / 2));
		} else {
			return checkOnce(n - 1, p, min(m / 2 - 1, (k + 1) / 2));
		}
	} else {
		if (k < m - 1) {
			return true;
		}
		return p == m;
	}
}

long long getMin(long long n, long long p) {
	long long l = 0, r = (1LL << n) - 1LL;
	while (r - l > 1LL) {
		long long m = (l + r) / 2LL;
		if (checkAlways(n, p, m)) {
			l = m;
		} else {
			r = m;
		}
	}

	if (checkAlways(n, p, r)) {
		return r;
	} else {
		return l;
	}
}

long long getMax(long long n, long long p) {
	long long l = 0, r = (1LL << n) - 1LL;
	while (r - l > 1LL) {
		long long m = (l + r) / 2LL;
		if (checkOnce(n, p, m)) {
			l = m;
		} else {
			r = m;
		}
	}

	if (checkOnce(n, p, r)) {
		return r;
	} else {
		return l;
	}
}

void solve(int test) {
	long long n, p;
	cin >> n >> p;
	cout << "Case #" << test << ": " << getMin(n, p) << " " << getMax(n, p) << endl;
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