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

VI g[1000111];
int n;

int vis[1000111];
int lim0, lim1;

int sal[1000111];


int go(int v) {
	if (sal[v] < lim0 || sal[v] > lim1) {
		return 0;
	}

	int n = g[v].size();
	int res = 1;
	FI {
		res += go(g[v][i]);
	}

	return res;
}


int solve() {
	GI(n);

	int D;
	LL S0, AS, CS, RS;
	LL M0, AM, CM, RM;
	cin >> D >> S0 >> AS >> CS >> RS >> M0 >> AM >> CM >> RM;
	
	LL mcur = M0, scur = S0;
	sal[0] = S0;
	FOR(i, 1, n) {
		scur = (scur*AS + CS) % RS;
		mcur = (mcur*AM + CM) % RM;
		g[mcur % i].push_back(i);
		sal[i] = scur;
	}
	lim0 = S0 - D;
	lim1 = S0 + D;


	int res = go(0);
	

	FI g[i].clear();

	return res;
}

void prepare_input();
int main() {
	prepare_input();
	
	int ntc; GI(ntc);
	FORE(tc, 1, ntc) {
		int res = solve();
		printf("Case #%d: %d\n", tc, res);
	}
}


void prepare_input()  {
	bool LOCAL = false;
	char TASK = 'A';

	static char in_file[200], out_file[200];
	if (LOCAL) {
		freopen("input.txt", "rt", stdin);
	} else {

		int ATTEMPT = 0;
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
