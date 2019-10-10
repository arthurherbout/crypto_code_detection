#include <iostream>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <utility>
#include <string>
#include <fstream>
#include <map>
#include <set>
#include <memory.h>
#include <cmath>
#include <queue>

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
#define GI(n) scanf("%d", &n)
#define GI2(n, m) scanf("%d %d", &n, &m)

#define N 8000000

int gcd(int a, int b) {
	while (a > 0 && b > 0) {
		if (a > b) a %=b; else b %=a;
	}
	return a|b;
}

LL f[N+1];

int n;
LL s;
int q[100];

bool fast_check() {
	int g = 0;
	FI g= gcd(g, q[i]);
	if (s % g) return false;
	return true;
}

LL go(LL s, int pos, LL lim) {
	if (s / q[pos] > lim) return -1;

	LL t = q[pos];
	if (pos == 0) return s % t ? - 1: s/t;
	if (s % t == 0) return s/t;
	
	LL pre = 0;
	if (s > t*t) {
		pre = (s - t*t) / t;
		s = s - pre*t;
		while (s > t*t) {
			s -= t; ++pre;
		}
	}
	lim -= pre;

	if (s <=N) return pre + f[s];

	LL res = -1;
	int take = 0;
	for (LL take = 0; take <= lim; ++take) {
		LL ss = s - s*take;
		LL rr;
		if(ss < 0) {
			rr = go(s, pos-1, lim);
		} else {
			rr =go(ss, pos-1, lim - take);
		}

		if (rr != -1) {
			if (res == -1 || res > rr + pre) {
				res = rr + pre;
			}
		}
		if (ss < 0) break;
	}

	return res;
}

LL solve() {
	cin >> s;
	cin >> n; FI cin >> q[i];
	sort(q, q+n);
	if (!fast_check()) return -1;

	memset(f, -1, sizeof f);
	f[0] = 0;
	FI {
		int t = q[i];
		FORE(j, t, N) if (f[j-t] != -1) {
			if (f[j] == -1 || f[j] > f[j-t] + 1)
				f[j]=  f[j-t] + 1;
		}
	}

	LL res = go(s, n-1, s);
	
	return res;
}

int main() {

freopen("B-small-attempt1.in", "rt", stdin);
freopen("B-small-attempt1.out", "w", stdout);

	int cases; GI(cases);
	FORE(ncase, 1, cases) {
		LL res = solve();
		if (res < 0) {
			cout << "Case #" << ncase <<": IMPOSSIBLE" << endl;
		} else {
			cout << "Case #" << ncase <<": ";
			cout << res << endl;
      }
	}
	
}
