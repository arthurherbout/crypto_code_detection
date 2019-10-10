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
#include <sstream>

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

set<int> E, F;

VI v[210];
int n;

int go(int pos) {
	if (pos == n)
		return 0;

	VI added;

	//English
	int res1 = 0;
	for (int id : v[pos]) {
		if (E.count(id))
			continue;
		if (F.count(id))
			++res1;

		E.insert(id);
		added.push_back(id);
	}
	res1 += go(pos + 1);
	for (auto id : added)
		E.erase(id);
	added.clear();

	//French
	int res2 = 0;
	for (int id : v[pos]) {
		if (F.count(id))
			continue;
		if (E.count(id))
			++res2;

		F.insert(id);
		added.push_back(id);
	}
	res2 += go(pos + 1);
	for (auto id : added)
		F.erase(id);


	return min(res1, res2);
}

int solve() {
	E.clear(); F.clear();
	string s,w;
	cin >> n;
	getline(cin, s);

	map<string, int> id;

	FI {
		getline(cin, s);
		istringstream iss(s);
		v[i].clear();
		while (iss >> w) {
			if (id.find(w) == id.end()) {
				int c = id.size();
				id[w] = c;
			}
			v[i].push_back(id[w]);
		}
	}

	int common = 0;
	FIR(v[0].size()) { E.insert(v[0][i]);}
	FIR(v[1].size()) { 
		F.insert(v[1][i]);
	}
	
	for (auto fr : F) {
		if (E.count(fr)) ++common;
	}

	int res = go(2);
	
	return res + common;
	
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
