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

int f[101][101][101][101];

const int UNUSED = 0x1f1f1f1f;
const int INF = UNUSED - 1;

int dh, da;
int kh, ka;
int ainc, adec;

int solve(int h, int a, int eh, int ea) {
	if (h <= 0)
		return INF;
	int& res = f[h][a][eh][ea];
	if (res != UNUSED)
		return res;

	res = INF;
	if (a >= eh)
		return res = 1;

	int t = INF;
	t= min(t, 1+solve(h - ea, a, eh - a, ea));
	t= min(t, 1 + solve(dh - ea, a, eh, ea));

	if (ainc)
		t = min(t, 1+solve(h - ea, min(eh, a + ainc), eh, ea));
	

	{
		int nea = max(0, ea - adec);
		t = min(t, 1 + solve(h - nea, a, eh, nea));
	}

	return res=t;
}

int solve() {
	memset(f, 0x1f, sizeof f);
	GI2(dh, da); GI2(kh, ka); GI2(ainc, adec);

	int res = solve(dh, da, kh, ka);
	if (res >= INF)
		res = -1;

	return res;
}

void prepare_input();
int main() {
	prepare_input();
	
	int ntc; GI(ntc);
	FORE(tc, 1, ntc) {
		int res = solve();
		
		if (res >= 0)
			printf("Case #%d: %d\n", tc, res);
		else
			printf("Case #%d: IMPOSSIBLE\n", tc);
		
	}
}


void prepare_input()  {
	bool LOCAL = false;
	char TASK = 'C';

	static char in_file[200], out_file[200];
	if (LOCAL) {
		freopen("input.txt", "rt", stdin);
	} else {

		int ATTEMPT = 2;
		bool LARGE = false;

		if (LARGE) {
			sprintf(in_file, "%c-large.in", TASK);
			sprintf(out_file, "%c-large.out", TASK);
		} else {
			sprintf(in_file, "%c-small-attempt%d.in", TASK,  ATTEMPT);
			sprintf(out_file, "%c-small-attempt%d.out", TASK,  ATTEMPT);
		}

		cerr << in_file <<  endl; freopen(in_file, "rt", stdin);
		cerr << out_file << endl; freopen(out_file, "w", stdout);
	}
}
