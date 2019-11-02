#define PROBLEM "D-small-attempt0"
#define PROBLEM_IN PROBLEM ".in"
#define PROBLEM_OUT PROBLEM ".out"

#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <queue>
#include <sstream>
#include <cassert>

using namespace std;
int GI() {int t; scanf("%d", &t); return t;}

#define LET(x,a) typeof(a) x(a)
#define REP(i,n) for (int i = 0; i < (n); ++ i)
#define FOR(i,a,b) for (int i = (a); i < (b); ++ i)
#define FORI(x,a,b) for (LET(x,a); x != (b); ++ x)
#define EACH(it,s) FORI(it, (s).begin(), (s).end())
#define INF (int)1e8
#define LINF (LL)1e16
#define dbg(x) cerr << #x << " -> " << (x) << "\t" << flush
#define dbge(x) cerr << #x << " -> " << (x) << "\n" << flush

typedef long long LL;
typedef pair<int,int> PII;
typedef vector<int> VI;
void solve();

#define cs c_str()
#define pb push_back
#define sz size()
#define mp make_pair

int s2i(string s) { int t; sscanf(s.cs, "%d", &t); return t; }
template<typename T> string tos(T x) { ostringstream oss; oss << x; return oss.str(); }

int main () {
	assert (freopen(PROBLEM_IN, "r", stdin));
	assert (freopen(PROBLEM_OUT, "w", stdout));
	int kases;
	for (int kase = 1, kases = GI(); kase <= kases; ++ kase) {
		printf ("Case #%d: ", kase);
		solve ();
	}
//	system ("pause");
	return 0;
}


int dp[4][4][1 << 16];
char grid [5][5];
int R, C;

int go(int r, int c, int f) {
	int &res = dp[r][c][f];
	if (res == -1) {
		res = 0;
		FOR(dx,-1,+2) FOR(dy,-1,+2) if (dx || dy) {
			int nr = r + dx, nc = c + dy;
			if (nr >= 0 && nc >= 0 && nr < R && nc < C && grid[nr][nc] != '#' && !(1&(f >> (nr*4+nc)))) {
				res |= ! go(nr, nc, f | (1 << (nr*4+nc)));
				if (res) return res;
			}
		}
	}
	return res;
}

void solve() {
	memset(dp, -1, sizeof(dp));
	R = GI(); C = GI();
	REP(r,R) scanf("%s", grid[r]);
	int rr, cc; REP(r,R) REP(c,C) if (grid[r][c] == 'K') rr = r, cc = c;
	puts (go(rr, cc, 1 << (rr*4+cc)) ? "A" : "B");
}