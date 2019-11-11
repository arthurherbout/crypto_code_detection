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

int n;

int X[1010], Y[1010], Z[1010];
int VX[1010], VY[1010], VZ[1010];

double d[1010][1010];

double L;
int vis[1010];

void dfs(int v) {
	if (vis[v])
		return;
	vis[v] = true;

	FI if (d[v][i] <= L)
		dfs(i);
}

bool can(double lim) {
	memset(vis, 0, sizeof vis);
	L = lim;
	dfs(0);
	return vis[1];
}

double solve() {
	int S;
	GI2(n, S);
	FI{
		GI2(X[i], Y[i]); GI(Z[i]);
		GI2(VX[i], VY[i]); GI(VZ[i]);
	}

	FI FJ{
		d[i][j] = sqrt((X[i] - X[j])*(X[i] - X[j]) + (Y[i] - Y[j])*(Y[i] - Y[j]) + (Z[i] - Z[j])*(Z[i] - Z[j]));
	}

	double lo = 0, hi = 1e30;
	FIR(200) {
		double mid = (lo + hi) / 2;
		if (can(mid))
			hi = mid;
		else
			lo = mid;
	}

	return lo;
}

void prepare_input();
int main() {
	prepare_input();
	
	int ntc; GI(ntc);
	FORE(tc, 1, ntc) {
		double res = solve();
		
		printf("Case #%d: %.7lf\n", tc, res);
		
	}
}


void prepare_input()  {
	bool LOCAL = false;
	char TASK = 'C';

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
