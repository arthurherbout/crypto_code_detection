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


VI primes;

bool is_prime(int n) {
	for (int i = 2; i*i <=n; ++i) if (n % i == 0) return false;
	return true;
}


vector<LL> v;
int n;

LL pw(LL a, int p, int mod) {
	if (p < 2) return p ? a : 1;
	LL t = pw(a, p/2, mod);
	t = (t*t) % mod;
	if (p & 1) t = (t*a) % mod;
	return t;
}

int rev(LL a, int p) {
	return pw(a, p-2, p);
}

int solve(int p) {
	if (v.size() < 3) {
		if(v.size() == 2 && v[0] == v[1]) return v[0];
		return -1;
	}

	if (v[0] == v[1]) {
		FIR(v.size()) if (v[i] != v[0]) return -1;
		return v[0];
	}

	LL r = (v[2] - v[1]) % p;
	if (r < 0) r += p;

	LL l = (v[1] - v[0]) % p;
	if (l < 0) l += p;

	LL A = (r* rev(l, p)) % p;
	LL B = (v[1] - A*v[0]) %p; if (B < 0) B += p;

	FOR(i, 1, v.size()) {
		if ( (A * v[i-1] + B) % p != v[i] ) return -1;
	}

	return (A * v.back() + B) % p;
}

int solve() {
	int d, n;
	GI2(d,n);
	v.clear();
	int k;

	FI { GI(k); v.push_back(k); }

	LL mn = *max_element(ALL(v));
	++mn;
	LL mx = 1;
	FIR(d) mx *= 10;

	int res = -1;
	int i = 0;
	while (primes[i] < mn) ++i;
	while (primes[i] <= mx) {
		int r = solve(primes[i++]);
		if (r == -1) continue;
		if (res == -1) {

			res =r;
		} else if (res != r) return -1;
	}

	return res;
}

int q[1000100];

int main() {

freopen("A-large.in", "rt", stdin);
freopen("A-large1.out", "w", stdout);
	memset(q, 0, sizeof q);
	FOR(i, 2, 2000) if (q[i] == 0) {
		for (int j = i+i; j < 1000100; j += i)
			q[j] = 1;
	}

	FOR(i, 2, 1000100) if (q[i ]== 0) primes.push_back(i);

	int cases; GI(cases);
	FORE(ncase, 1, cases) {
		int res = solve();

		if (res < 0) {
			printf("Case #%d: I don't know.\n", ncase, res);
		} else
			printf("Case #%d: %d\n", ncase, res);
	}
}
