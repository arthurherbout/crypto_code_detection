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
#include<cassert>

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
#define GI2(n,m) scanf("%d %d", &n, &m)

vector<LL> v;
int lens[100];

double solve() {	
	LL B; cin >> B;
	int n; GI(n);
	v.assign(37, 0);
	FI cin >> v[i];
	

	sort(ALL(v));
	double res = 0;
	v.push_back((LL)10e16);
	
	int p = 0, pe = 0;
	while (p < 37) {
		pe = p; while (v[pe] == v[p]) ++pe;
		lens[p] = pe-p;
		p = pe;
	}
	lens[37] = 1;

	LL taken = 0;
	for(p = 0; p <= 37; p += lens[p]) {
		LL needEq = 0; FIR(p) needEq += v[p] - v[i];

		if (needEq <= B) {
			double k = p + lens[p];
			double r = needEq * (36.0/k - 1);
			res = max(res, r);

			if (p) {
				FORE(j, 1, lens[p]) if (needEq + j <= B) {
					k = p+lens[p] - j;
					r = needEq * (36.0/k - 1) - j;
					res = max(res, r);
				}
			}
		}

		if(p && B >= taken) {
			LL add = (B - taken) / p;
			if (v[p-1] + add >= v[p]) {
				add = v[p] - v[p-1] - 1;
			}

			LL lev = v[p-1] + add;
			FORE(i, 1, p) {
				LL T = 0;
				LL off = 0;
				FJR(i) T += lev - v[j];
				FOR(j, i, p) off += lev+1 - v[j];
				if (T + off <= B) {
					double r = T*(36.0/i - 1) - off;
					res = max(res, r);
				}
			}

			if (add) {
				LL lev = v[p-1] + add-1;
				FOR(i, 1, p) {
					LL T = 0;
					LL off = 0;
					FJR(i) T += lev - v[j];
					FOR(j, i, p) off += lev+1 - v[j];
					if (T + off <= B) {
						double r = T*(36.0/i - 1) - off;
						res = max(res, r);
					}
				}
			}
		}

		taken = needEq;
	}


	return res;

}

int main() {
//freopen("input.txt", "rt", stdin);
freopen("A-large.in", "rt", stdin); freopen("A-large.out", "w", stdout);
	
	int ntc; GI(ntc);
	FORE(tc, 1, ntc) {
		double res = solve();
		printf("Case #%d: %.10lf\n", tc, res);
	}

}
