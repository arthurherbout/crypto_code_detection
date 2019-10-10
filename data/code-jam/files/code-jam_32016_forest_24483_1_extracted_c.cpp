#include <vector>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <map>
#include <queue>
#include <set>
#include <functional>

using namespace std;

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

typedef pair<int, int> PI;
typedef vector<PI> VPI;

struct M {
	int a[2][2];
	M operator*(M m) {
		M res;
		memset(&res, 0, sizeof res);
		res.a[0][0] = (a[0][0]*m.a[0][0] +  a[0][1]*m.a[1][0]) % 1000;
		res.a[0][1] = (a[0][0]*m.a[0][1] +  a[0][1]*m.a[1][1]) % 1000;
		res.a[1][0] = (a[1][0]*m.a[0][0] +  a[1][1]*m.a[1][0]) % 1000;
		res.a[1][1] = (a[1][0]*m.a[0][1] +  a[1][1]*m.a[1][1]) % 1000;

		return res;
	}
};

M pp(M m, int n) {
	if (n == 1) return m;
	M t = pp(m, n/2);
	t = t*t;
	if (n % 2)
		t = t*m;
	return t;
}

struct Task {
	int solve(int n) {
		M m;
		m.a[0][0] = 6;
		m.a[0][1] = -4;
		m.a[1][0] = 1;
		m.a[1][1] = 0;

		m = pp(m, n);
		return (m.a[1][0]*6 + m.a[1][1]*2) % 1000;
	}
};

int main() {
freopen("C-large.in", "rt", stdin);
freopen("C-large.out", "w", stdout);

	int tc; cin >>tc;
	REP(TC, tc) {
		Task t;
		int N;
		cin >> N;
		int res = t.solve(N);
		if (res <= 0) res += 1000;
		--res;
		char buf[1000];
		if (res < 10) sprintf(buf, "00%d", res); 
		else if (res < 100) sprintf(buf, "0%d", res);
		else sprintf(buf, "%d", res);
		printf("Case #%d: %s\n", TC+1, buf);
	}

fclose(stdout);
}
