#include <iostream>
#include <vector>
#include <string>
#include <memory.h>
#include <algorithm>
#include <set>
#include <queue>
#include <sstream>
#include <list>
#include <map>
#include <cmath>

#include <memory.h>

using namespace std;

#define FOR(i,a,b) for(int i = (a); i < (b); ++i)
#define FORE(i,a,b) for(int i = (a); i <= (b); ++i)
#define FORD(i,a,b) for(int i = (a); i >= (b); --i)
#define FIR(k) FOR(i,0,k)
#define FJR(k) FOR(j,0,k)
#define FI FIR(n)
#define FJ FJR(n)
#define ALL(v) v.begin(), v.end()
#define LL long long

typedef vector<int> VI;
typedef pair<int, int> PI;

int n;
// x, y;
PI p[1100];
int y_cnt[40];

bool can_1() {
	FOR(y, 1, 40) if ((y_cnt[y] > 1) || (y_cnt[y] && y_cnt[y-1])) return false;
	return true;
}

bool can_2() {
	FOR(y, 1, 39) if (y_cnt[y]) {
		if (y_cnt[y] == 1)
			for(int dy = -1; dy <= 1; dy += 2) if (y_cnt[y + dy] > 1) return false;
		if (y_cnt[y] > 1)
			for(int dy = -1; dy <= 1; dy += 2) if (y_cnt[y + dy] > 0) return false;
	}
	return true;
}

map<PI, int> lf[3], rt[3];

bool can_3() {
	VI y[35];
	int pos[35] = {};
	FIR(3) lf[i].clear(),	rt[i].clear();

	FI y[ p[i].second ].push_back(p[i].first);
	FIR(35) sort(ALL(y[i]));

	while (true) {
		int yy = -1;
		FIR(35) if (pos[i] < y[i].size()) {
			if (yy == -1 || y[yy][ pos[yy] ] > y[i][pos[i]]) 
				yy = i;
		}
		if (yy == -1) break;
		int xx = y[yy] [pos[yy]];

		FIR(35) while (pos[i] < y[i].size() && y[i][pos[i]] <= xx) ++pos[i];
		for (int dy = -1; dy <= 1; ++dy) if(pos[yy+dy] < y[yy+dy].size()) {
			int ny = yy+dy;
			int nx = y[ny][pos[ny]];

			++rt[1+dy][PI(yy, xx)];
			++lf[1+dy][PI(ny, nx)];
		}
	}

	FI {
		int ll = lf[0][p[i]];
		int rr = rt[0][p[i]];
		if (ll > 1 && rr > 0) return false;

		ll = lf[2][p[i]];
		rr = rt[2][p[i]];
		if (ll > 1 && rr > 0) return false;
	}

	return true;
}

int solve() {
	cin >> n;
	FI cin >> p[i].first >> p[i].second;
	memset(y_cnt, 0, sizeof y_cnt);

	FI ++y_cnt[ p[i].second ] ;

	if (can_1()) return 1;
	if (can_2()) return 2;

	if (can_3()) 
		return 3;

	return 4;
}

int main() {
static const string FILENAME = "C-small-attempt1";
freopen((FILENAME + ".in").c_str(), "rt", stdin);
freopen((FILENAME + ".out").c_str(), "w", stdout);	
	
	int ncase; cin >> ncase;
	FORE(caze, 1, ncase) {
		int res = solve();
		printf("Case #%d: %d\n", caze, res);
	}

}