#include <iostream>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <utility>
#include <string>
#include <fstream>
#include <sstream>
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
int dep[110];
int pos[110];
char fl[110];

string words[10];

int w[110];

set<LL> seen;

void fix() {
	FI w[pos[i]] = i;
	bool ok;
	do {
		ok = true;

		FI if (dep[pos[i]] >= 0 && w[dep[pos[i]]] > i) {
			int j = w[dep[pos[i]]];
			swap(pos[i], pos[j]);
			swap(w[pos[i]], w[pos[j]]);
			ok = false;
		}

	} while (!ok);
}

bool isok() {
	FI w[pos[i]] = i;
	FI if (dep[pos[i]] >= 0 && w[dep[pos[i]]] > i)
		return false;
	return true;
}

LL hfash() {
	LL res = 0;
	FI{
		res += 1370447 * res + pos[i];
	}

	return res;
}

void solve() {
	GI(n);
	FI GI(dep[i]);
	FI --dep[i];
	string s(n, ' ');
	scanf("%s", fl);

	int ws;
	GI(ws);
	FIR(ws) cin >> words[i];

	int iter = 2200 * n;
	int count[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0};

	seen.clear();
	int g = 0;
	REP(cii, iter) {
		FI pos[i] = i;
		random_shuffle(pos, pos + n);
		fix();

	//	LL h = hfash();
		//if (seen.count(h)) continue;
		//seen.insert(h);
		FI s[i] = fl[pos[i]];
		++g;

		FIR(ws) if (s.find(words[i]) != string::npos)
			++count[i];
	}

	FIR(ws) {
		printf(" %.2lf", (double)count[i] / g);
	}
	puts("");

}

void prepare_input();
int main() {
	prepare_input();
	
	int ntc; GI(ntc);
	FORE(tc, 1, ntc) {
		printf("Case #%d:", tc);
		
		fprintf(stderr, "Case #%d:\n", tc);
		solve();
		
	}
}


void prepare_input()  {
	bool LOCAL = false;
	char TASK = 'B';

	static char in_file[200], out_file[200];
	if (LOCAL) {
		freopen("input.txt", "rt", stdin);
	} else {

		int ATTEMPT = 5;
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
