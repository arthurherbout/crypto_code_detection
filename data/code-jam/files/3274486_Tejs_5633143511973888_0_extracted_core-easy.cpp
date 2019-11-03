#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <cmath>

using namespace std;

typedef long double ld;
typedef vector<ld> vld;

ld findThr(vld& ps, int n, ld u) {
	ld lo = 0, hi = 1;
	for(int t = 0;t < 200; ++t) {
		ld mid = (lo+hi)/2;
		ld sum = 0;
		for(int i = 0;i < n; ++i) {
			if(ps[i] < mid) sum += mid - ps[i];
		}
		if(sum > u) {
			hi = mid;
		} else {
			lo = mid;
		}
	}
	return lo;
}

ld solve() {
	int n, k;
	cin >> n >> k;
	//asserting n == k
	ld u;
	cin >> u;
	vld ps(n);
	for(int i = 0;i < n; ++i) cin >> ps[i];
	ld thr = findThr(ps, n, u);
	ld logProb = 0;
	for(int i = 0;i < n; ++i) {
		ld t = ps[i] < thr ? thr : ps[i];
		logProb += log(t);
	}
	return exp(logProb);
}

int main() {
	cout << fixed << setprecision(10);
	int T;
	cin >> T;
	for(int t = 1;t <= T; ++t) {
		cout << "Case #" << t << ": " << solve() << endl;
	}
	return 0;
}