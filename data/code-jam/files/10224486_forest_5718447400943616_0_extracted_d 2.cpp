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

int can[35][35];
int n;
char buf[1000];

int test[35][35];
int taken[4];

bool good_setup() {
	int arrive[4] = { 0, 1, 2, 3 };
	do {
		int choose[4] = { 0, 1, 2, 3};

		bool found = false;
		do {
			
			FI taken[i] = false;
			bool ok = true;
			FI {
				int p = arrive[i];
				
				bool has_any = false;
				FJ if (test[p][j] && !taken[j]) has_any = true;
				if (!has_any)
					return false;

				if (!test[p][choose[i]]) {
					ok = false;
					break;
				}

				taken[choose[i]] = true;

			}
		} while (next_permutation(choose, choose + n));

	} while (next_permutation(arrive, arrive + n));

	return true;
}


int solve() {
	GI(n);
	FI{
		scanf("%s", buf);
		FJ can[i][j] = buf[j] == '1';
	}

	int res = 10000;

	REP(mask, 1 << (n*n)) {
		REP(w, n) REP(j, n) {
			int v = mask & (1 << (w*n + j));
			test[w][j] = v == 0 ? 0 : 1;
		}

		bool ok = true;
		FI FJ if (test[i][j] < can[i][j])
			ok = false;
		if (!ok) continue;

		int cost = 0;
		FI FJ cost += (test[i][j] - can[i][j]);

		if (cost < res && good_setup()) {
			res = cost;
		}
	}

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
	char TASK = 'D';

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
