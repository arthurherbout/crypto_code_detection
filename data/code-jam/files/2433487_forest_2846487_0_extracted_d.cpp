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

#define GI(n) scanf("%d", &n)
#define GI2(n,m) scanf("%d %d", &n, &m)

char buf[100];
int n;

double f[1 << 20];

#define SET(m,x) ((m) & ( 1<<(x) ))

double go(int mask) {
	double& res = f[mask];
	if(res >= 0) return res;

	res = 0;
	int q=0; while (SET(mask, q))++q;
	int pos = q;
	FORD(i, n-1, 0) {
		++q;
		if (!SET(mask, i)) q = 0,pos= i;
		res += n-q + go(mask | (1 << pos));
	}
	res /= n;
	return res;
}

double solve() {
	scanf("%s", buf);
	n= strlen(buf);
	int mask = 0;
	int togo = 0; FI if (buf[i] == '.') ++togo; else mask |= 1 << i;
	FIR(1 << 20) f[i] = -1;
	f[(1 << n)-1] =0;

	return go(mask);
}

int main() {
//freopen("input.txt", "rt", stdin);
freopen("D-small-attempt0.in", "rt", stdin);
freopen("D-small-attempt0.out", "w", stdout);
	
	int ntc; GI(ntc);
	FORE(tc, 1, ntc) {
		double res = solve();
		printf("Case #%d: %.12lf\n", tc, res);
	}

}
