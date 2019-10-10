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

string d[530500];
int Dcnt;
string s;
int n;
int f[55][6];


int go(int pos, int b) {
	int &res = f[pos][b];
	if (res >= 0)
		return res;
	if (pos == n)
		return res =0;

	res = 100000000;
	FIR(Dcnt) if (pos +  d[i].size() <= n) {
		int k = b;
		int cnt = 0;
		for (int len = d[i].size(), j = 0; j < len; ++j)
			if (s[pos+j] == d[i][j]) --k;
			else {
				if (k > 0) {
					cnt = -1;
					break;
				}
				++cnt;
				k = 4;
			}

		if (cnt >= 0) {
			res = min(res, cnt + go(pos + d[i].size(), max(0, k)));
		}
	}

	return res;
}

int solve() {
	memset(f, -1, sizeof f);
	cin >> s;
	n = s.size();

	int res = go(0,0);
	return res;
}


void init() {
	ios_base::sync_with_stdio(false);
	ifstream f("garbled_email_dictionary.txt");
	string s;
	while (f >> s) {
		d[Dcnt++] = s;
	}
}

int main() {
init();

//freopen("input.txt", "rt", stdin);
freopen("C-small-attempt0.in", "rt", stdin);
freopen("C-small-attempt0.out", "w", stdout);

	int ntc; GI(ntc);
	FORE(tc, 1, ntc) {
		int res = solve();
		printf("Case #%d: %d\n", tc, res);
	}
}
