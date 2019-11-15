#include <iostream>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <utility>
#include <string>
#include <fstream>
#include <map>
#include <set>
#include <queue>
#include <memory.h>

using namespace std;

typedef vector<int> VI;
typedef pair<int, int> PI;
typedef vector<PI> VPI;

#define FOR(i,a,n) for (int i = (a); i < (n); ++i)
#define FORE(i,a,n) for (int i = (a); i <= (n); ++i)
#define FORD(i,a,b) for (int i = (a); i >= (b); --i)
#define REP(i,n) FOR(i,0,n)
#define REPE(i,n) FORE(i,0,n)
#define LL long long
#define FIR(n) REP(i,n)
#define FJR(n) REP(j,n)
#define ALL(v) v.begin(), v.end()

#define FI FIR(n)
#define FJ FJR(n)
#define FR(i,a) FOR(i,a,n)
#define REPN(i) REP(i,n)

#define N 1000002

int win[N+1];
int win2[N+1];

int winning(int a, int b) {
	if (b < win[a-1]) return true;

	for (int k = 1; a - k*b > 0; ++k) if (a-k*b >= win[b]) return true;
	return false;
}

int main() {
freopen("C-large.in", "rt", stdin);
freopen("C-large.out", "w", stdout);

	win[0] = win[1] = 0;
	FORE(a, 2, N) {
		int l = 1, r = a;
		while (l < r) {
			int m = (l+r)/2;
			if(winning(a, m)) 
				l = m+1; 
			else 
				r = m;
		}
		win[a] = l;
	}
	FORE(i, 0, N) win2[i] = 1000000000;
	int last = 0;
	FORE(i, 2, N) {
		FOR(j, last, win[i]) win2[j] = i;
		last = win[i];
	}

	int tests, a1, a2, b1, b2;
	cin >> tests;
	FORE(T, 1, tests) {
		cin >> a1 >> a2 >> b1 >> b2;
		LL res = 0;
		FORE(a, a1, a2) {
			if (b1 <= a) {
				int d = min(win[a] - 1, b2) - b1 + 1;
				if (d > 0) res += d;
			}

			if (b2 >= a) {
				int d = b2 - max(win2[a], b1) + 1;
				if (d > 0) res += d;
			}
		}

		printf("Case #%d: %lld\n", T, res);
	}
}
