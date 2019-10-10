//N = K
#include <cstdio>
#include <iostream>
#include <algorithm>
using namespace std;

int t;
int n, k;
double u, p[50];

double getCost(double x) {
	double ret = 0;
	for (int i = 0; i < n; i++) {
		if (p[i] < x) ret += x - p[i];
	}
	return ret;
}

double solve() {
	double st = 0, ed = 1, mid;
	for (int i = 0; i < 100; i++) {
		mid = (st + ed) / 2;
		if (getCost(mid) <= u) st = mid;
		else ed = mid;
	}
	
	double ans = 1.0;
	for (int i = 0; i < n; i++) {
		ans *= max(p[i], st);
	}
	return ans;
}

signed main() {
	cin >> t;
	for (int id = 1; id <= t; id++) {
		cin >> n >> k >> u;
		for (int i = 0; i < n; i++) cin >> p[i];
		double ans = solve();
		printf("Case #%d: %f\n", id, ans);
	}
	return 0;
}