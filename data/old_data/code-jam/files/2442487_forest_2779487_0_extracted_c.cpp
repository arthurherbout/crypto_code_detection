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

VI g[2010];
int n;
int A[2010], B[2010];

int f[2010];

int go(int v) {
	int &res = f[v];
	if (res >= 0)
		return res;

	res = 0;
	FIR(g[v].size())
		res = max(res, 1 + go(g[v][i]));
	return res;
}

void solve() {
	GI(n);
	FORE(i, 1, n) GI(A[i]);
	FORE(i, 1, n) GI(B[i]);

	FORE(i, 1, n) g[i].push_back(0);

	FORE(i, 1, n) FOR(j, 1, i) if (A[i] <= A[j]) g[j].push_back(i);
	FORE(i, 1, n) FOR(j, i+1, n) if (B[i] <= B[j]) g[j].push_back(i);


	memset(f, -1, sizeof f);
	f[0] = 0;
	FORE(i, 1, n) go(i);

	set<int> used;
	FORE(i, 1, n) {
		int j = f[i];
		while (used.count(j)) ++j;
		used.insert(j);
		printf(" %d", j);
	}

	puts("");
	REPE(i,n) g[i].clear();

}

int main() {
//freopen("input.txt", "rt", stdin);
freopen("C-small-attempt0.in", "rt", stdin);
//freopen("C-small-attempt0.out", "w", stdout);

	int ntc; GI(ntc);
	FORE(tc, 1, ntc) {
		printf("Case #%d:", tc);
		solve();
	}	
}
