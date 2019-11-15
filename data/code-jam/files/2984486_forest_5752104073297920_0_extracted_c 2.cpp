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

int p[1010];
double f[1010][1010];

void gen(int n) {
	FI FJ f[i][j] = i == j;
	double inv = 1.0/n;
	double inv1 = 1 - inv;

	FOR(pos, 0, n) {
		FI if (i!=pos) FJ {
			f[i][j] = inv1*f[i][j] + inv*f[pos][j];
		}
		FJ f[pos][j] = inv;
	}
}

bool solve(int n) {
	FI GI(p[i]);

	double pr = 0;
	FI pr += f[i][p[i]];
	
	//printf("%.5lf ", pr);
	return pr > 1.005;
}

void prepare_input();
int main() {
	prepare_input();
	
	int N;
	int ntc = 0;
	
	GI(ntc);
	

	FORE(tc, 1, ntc) {
		GI(N);
		if (tc == 1) {
			gen(N);
		}
		int res = solve(N);
		printf("Case #%d: %s\n", tc, (res ? "BAD" : "GOOD"));
	}
	return 0;
}


void prepare_input()  {
	bool LOCAL = false;
	char TASK = 'C';

	static char in_file[200], out_file[200];
	if (LOCAL) {
		freopen("good.txt", "rt", stdin);
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
