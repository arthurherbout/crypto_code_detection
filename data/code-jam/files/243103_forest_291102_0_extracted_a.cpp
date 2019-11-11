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
typedef vector<PI> VPI;

#define WALL 2

int source[12][12], dest[12][12];
VPI v, v_dest;
int R, C;

typedef PI State;
int b[5][5];

State get_state(const VPI& v) {
	int x = 100, y = 100;
	FIR(v.size()) y= min(y, v[i].first), x = min(x, v[i].second);
	memset(b, 0, sizeof b);
	FIR(v.size()) b[v[i].first - y][v[i].second - x] = 1;
	int pos = (x << 8) | y;
	int mask = 0;
	int p = 0;
	for (int i = 0; i < 5; ++i) for (int j = 0; j < 5; ++j, ++p) if (b[i][j]) mask |= 1 << p;
	return PI(pos, mask);
}

int play[12][12];

void decode(State s, VPI& v) {
	v.clear();
	memcpy(play, source, sizeof play);
	int m = s.second;
	int x = s.first >> 8, y = s.first & 255;
	int p = 0;
	for (int i = 0; i < 5; ++i) for (int j = 0; j < 5; ++j, ++p) if (m & (1 << p)) {
		v.push_back(PI(i+y, j+x));
		play[i+y][j+x] = 1;
	}
}

map<State, int> vis;

int dx[] = {-1, 1, 0, 0};
int dy[] = {0, 0, -1, 1};

int tt[12][12];

void go(int y, int x) {
	if (tt[y][x] != 1) return;
	tt[y][x] = 2;
	FIR(4) {
		int yy = y +dy[i], xx = x+ dx[i];
		if(xx<0 || yy < 0 || yy >= R || xx >= C) continue;
		go(yy, xx);
	}
}

bool connected(VPI& v) {
	memset(tt, 0, sizeof tt);
	FIR(v.size()) tt[v[i].first][v[i].second] = 1;
	int r = 0;
	go(v[0].first, v[0].second);
	FIR(R) FJR(C) if (tt[i][j] == 2) ++r;
	return r == v.size();
}

int solve() {
	cin >> R >> C;
	string s;
	v.clear(); v_dest.clear();
	memset(source, 0, sizeof source);
	memset(dest, 0, sizeof dest);
	FIR(R) {
		cin >> s;
		FORE(c, 0, C) {
			if (s[c] == '#') {
				source[i][c] = dest[i][c] = WALL;
			} else if (s[c] == 'x') {
				dest[i][c] = 1;
			} else if (s[c] == 'o') {
				source[i][c] = 1;
			} else if (s[c] == 'w'){
				source[i][c] = dest[i][c] = 1;
			}

			if (source[i][c] == 1) {
				v.push_back(PI(i, c));
			}

			if (dest[i][c] == 1) {
				v_dest.push_back(PI(i, c));
			}
		}
	}

	State start = get_state(v);
	State finish = get_state(v_dest);

	FIR(12) FJR(12) if (source[i][j] != 2) source[i][j] = 0;

	vis.clear();
	queue<State> q;

	vis[start] = 0;
	q.push(start);

	while (!q.empty()) {
		State s = q.front(); q.pop();
		if (s == finish) 
			return vis[s];
		
		int steps = vis[s];
		decode(s, v);
		bool con = connected(v);

		FIR(v.size()) {
			int y = v[i].first, x = v[i].second;
			if (y > 0 && y+1 < R && !play[y-1][x] && !play[y+1][x]) {
				++v[i].first;
				State new_state = get_state(v);
				if ((con || connected(v)) && !vis.count(new_state)) {
					vis[new_state]= steps+1;
					q.push(new_state);
				}

				v[i].first -= 2;
				new_state = get_state(v);
				if ((con || connected(v)) && !vis.count(new_state)) {
					vis[new_state]= steps+1;
					q.push(new_state);
				}
				++v[i].first;
			}

			if (x > 0 && x+1 < C && !play[y][x-1] && !play[y][x+1]) {
				++v[i].second;
				State new_state = get_state(v);
				if ((con || connected(v)) && !vis.count(new_state)) {
					vis[new_state]= steps+1;
					q.push(new_state);
				}

				v[i].second -= 2;
				new_state = get_state(v);
				if ((con || connected(v)) && !vis.count(new_state)) {
					vis[new_state]= steps+1;
					q.push(new_state);
				}
				++v[i].second;
			}
		}
	}

	return -1;
}

int main() {
static const string FILENAME = "A-small-attempt0";
freopen((FILENAME + ".in").c_str(), "rt", stdin);
freopen((FILENAME + ".out").c_str(), "w", stdout);	
	
	int ncase; cin >> ncase;
	FORE(caze, 1, ncase) {
		int res = solve();
		printf("Case #%d: %d\n", caze, res);
	}

}