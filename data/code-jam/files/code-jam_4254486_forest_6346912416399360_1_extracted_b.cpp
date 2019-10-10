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


int mint[1010], maxt[1010];
int last[1010];

int solve() {
	int n, k; GI2(n, k);
	memset(mint, 0, sizeof mint); memset(maxt, 0, sizeof maxt);
	memset(last, 0, sizeof last);

	int lastSum;  GI(lastSum);
	int curSum; 
	LL firstSum = lastSum;

	FOR(i, 0, n - k) {
		GI(curSum);
		int p = i % k;
		last[p] += (curSum - lastSum);
		lastSum = curSum;

		mint[p] = min(mint[p], last[p]);
		maxt[p] = max(maxt[p], last[p]);
	}

	int res = 0;
	LL sum = 0;
	int gmax = 0;
	REP(i, k) {
		maxt[i] -= mint[i];
		sum -= mint[i];
		gmax = max(maxt[i], gmax);
	}

	res = gmax;

	firstSum -= sum;
	firstSum %= k;
	
	if (firstSum < 0) firstSum += k;


	LL up = 0;
	REP(i, k) {
		up += gmax - maxt[i];
	}

	if (firstSum == 0 || up >= firstSum)
		return res;
	
	return res+1;
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
	char TASK = 'B';

	static char in_file[200], out_file[200];
	if (LOCAL) {
		freopen("input.txt", "rt", stdin);
	} else {
			
		int ATTEMPT = 6;
		bool LARGE = true;

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
