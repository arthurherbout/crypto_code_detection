#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <algorithm>
using namespace std;

int R, C;
vector<string> c;
map<int, pair<int,int> > d;

int gi, gj;
set<pair<int,int> > visited;
vector<pair<int,int> > a;
set<long long> visiteds;

bool visit(int i, int j) {
	if (i == gi && j == gj) return true;
	if (c[i][j] == '#') return false;
	if (!visited.insert(make_pair(i,j)).second) return false;
	if (visit(i+1, j)) return true;
	if (visit(i, j+1)) return true;
	if (visit(i, j-1)) return true;
	return false;
}

bool check(long long s) {
	for (int i = 0; i < (int)a.size(); ++ i) {
		int x = a[i].first-1;
		int y = a[i].second-1;
		if (!((s>>(x*8+y))&1)) return false;
	}
	return true;
}

long long hash(const set<pair<int,int> >& s) {
	long long r = 0;
	for (set<pair<int,int> >::const_iterator i = s.begin(); i != s.end(); ++ i) {
		int x = i->first-1;
		int y = i->second-1;
		r |= 1LL<<(x*8+y);
	}
	return r;
}

bool undo(long long s) {
	if (!visiteds.insert(s).second) return false;
	if (check(s)) return true;
	int di[] = {1, 0, 0};
	int dj[] = {0, +1, -1};
	for (int k = 0; k < 3; ++ k) {
		set<pair<int,int> > ss;
		for (int i = 0; i < R; ++ i) {
			for (int j = 0; j < C; ++ j) {
				if (c[i][j] != '#') {
					int ii = i+di[k];
					int jj = j+dj[k];
					if (c[ii][jj] == '#') {
						ii = i; jj = j;
					}
					-- ii;
					-- jj;
					if ((s>>(ii*8+jj))&1) {
						ss.insert(make_pair(i, j));
					}
				}
			}
		}
		if (undo(hash(ss))) return true;
	}
	return false;
}

pair<int, bool> func(int p) {
	gi = d[p].first;
	gj = d[p].second;
	a.clear();
	for (int i = 0; i < R; ++ i) {
		for (int j = 0; j < C; ++ j) {
			if (c[i][j] != '#') {
				visited.clear();
				if (visit(i, j)) a.push_back(make_pair(i,j));
			}
		}
	}
	set<pair<int,int> > s;
	s.insert(make_pair(gi, gj));
	visiteds.clear();
	return make_pair((int)a.size(), undo(hash(s)));
}

int main() {
	int T;
	cin >> T;
	for (int tt = 1; tt <= T; ++ tt) {
		cin >> R >> C;
		c = vector<string>(R);
		d.clear();
		for (int i = 0; i < R; ++ i) {
			cin >> c[i];
			for (int j = 0; j < C; ++ j) {
				if ('0' <= c[i][j] && c[i][j] <= '9') {
					d[c[i][j]-'0'] = make_pair(i,j);
				}
			}
		}
		printf("Case #%d:\n", tt);
		for (int i = 0; i < (int)d.size(); ++ i) {
			pair<int, bool> ans = func(i);
			printf("%d: %d %s\n", i, ans.first, ans.second ? "Lucky" : "Unlucky");
		}
	}
}
