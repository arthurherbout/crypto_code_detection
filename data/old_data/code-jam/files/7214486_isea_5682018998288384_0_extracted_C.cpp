// GCJ
#include <cmath>
#include <ctime>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <bitset>
#include <queue>
#include <map>
#include <set>

using namespace std;

// Self Template Code BGEIN

#define sz(x) ((int)((x).size()))
#define out(x) printf(#x" %d\n", x)
#define rep(i,n) for (int i = 0; i < (n); ++i)
#define repf(i,a,b) for (int i = (a); i <= (b); ++i)
#define repd(i,a,b) for (int i = (a); i >= (b); --i)
#define repcase int t, Case = 1; for (scanf ("%d", &t); t; --t)
#define repeach(i,x) for (__typeof((x).begin()) i = (x).begin(); i != (x).end(); ++i)

typedef long long int64;
typedef pair<int, int> pii;

int sgn(double x) { return (x > 1e-8) - (x < -1e-8); }
int count_bit(int x) { return x == 0? 0 : count_bit(x >> 1) + (x & 1); }

template<class T> inline void ckmin(T &a, const T b) { if (b < a) a = b; }
template<class T> inline void ckmax(T &a, const T b) { if (b > a) a = b; }

// Self Template Code END

const int MAXN = 10000 + 10;

vector<int> g[MAXN];
int c[MAXN], n;
int val[MAXN], valid[MAXN];

int get_col(char ch) {
	return ch - 'A';
}

void input() {
	scanf ("%d", &n);
	rep (i, n) {
		char buf[2];
		scanf ("%s", buf);
		c[i] = get_col(buf[0]);
	}
	rep (i, n) {
		g[i].clear();
	}
	rep (i, n - 1) {
		int u, v;
		scanf ("%d%d", &u, &v);
		g[--u].push_back(--v);
		g[v].push_back(u);
	}
}

const int BEGIN = 2012;
const int MUVAL = 9997;
const int MOD = 199008;

bool poss;

int get_val(int x) {
	return (x ^ (BEGIN * x)) % MOD;
}

// 0 - invalid
// 1 - valid
// 2 - valid on centre

bool try_(vector<pii> s, int idx, int range) {
	for (int i = idx; i + 1 < range; i += 2) {
		if (s[i].first != s[i + 1].first) {
			return false;
		}
		if (valid[s[i].second] == 2 || valid[s[i + 1].second] == 2) {
			return false;
		}
	}
	return true;
}

int croot;

void check(int x, int f) {
	vector<pii> sons;
	repeach (it, g[x]) {
		if (*it != f) {
			sons.push_back(pii(val[*it], *it));
		}
	}
	sort (sons.begin(), sons.end());
	if (sz(sons) % 2 == 0) {
		if (!try_(sons, 0, sz(sons))) {
		// 	printf ("WAAA %d\n", x);
		// repeach (it, sons) {
		// 	printf ("(%d, %d %d) ", it->first, it->second,
		// 		get_val(c[it->second]));
		// }
		// puts ("");
			poss = false;
			return ;
		}
		// valid[x] = 1;
	} 
}

void dfs(int x, int f) {
	// printf ("dfs %d %d %d\n", x, f, sz(g[x]));
	if (!poss) return ;
	if (sz(g[x]) == 1 && f != -1) {
		valid[x] = 1;
		val[x] = get_val(c[x]);
		// puts ("return");
		return ;
	}
	vector<pii> sons;
	repeach (it, g[x]) {
		if (*it != f) {
			dfs(*it, x);
			if (!poss) {
				return ;
			}
			sons.push_back(pii(val[*it], *it));
		}
	}
	sort (sons.begin(), sons.end());
	// printf ("check %d %d\n", x, croot);
	if (x == croot) {
		// must centred
		int vp = 0, i;
		for (i = 0; i < sz(sons) - 1; ++i) {
			if (sons[i].first != sons[i + 1].first) {
				valid[sons[i].second] = 2;
				check(sons[i].second, x);
				vp += 1;
			} else {
				++i;
			}
		}
		if (i == sz(sons) - 1) {
			valid[sons[i].second] = 2;
			check(sons[i].second, x);
			vp += 1;
		}
		if (vp > 2) {
			// printf ("WAAAc %d\n", x);
			// repeach (it, sons) {
			// 	printf ("(%d, %d %d) ", it->first, it->second,
			// 		get_val(c[it->second]));
			// }
			// puts ("");
			poss = false;
			return ;
		}
		// printf ("here %d\n", x);
		return ;
	}
	if (sz(sons) % 2 == 0) {
		// if (!try_(sons, 0, sz(sons))) {
		// 	printf ("WAAA %d\n", x);
		// 	poss = false;
		// 	return ;
		// }
		valid[x] = 1;
	} else {
		if (sz(sons) == 1) {
			valid[x] = 2;
		} else {
			if (!try_(sons, 0, sz(sons)) && !try_(sons, 1, sz(sons))) {
				// printf ("WAAA %d\n", x);
				// repeach (it, sons) {
				// 	printf ("(%d, %d %d) ", it->first, it->second,
				// 		get_val(c[it->second]));
				// }
				// puts ("");
				poss = false;
				return ;
			}
			valid[x] = 2;
		}
	}
	int res = get_val(c[x]);
	repeach (it, sons) {
		res = ((res * MUVAL) ^ it->first) % MOD;
	}
	val[x] = res;
}

bool rooted(int root) {
	memset (valid, 0, sizeof(valid));
	poss = true;
	croot = root;
	dfs (root, -1);
	return poss;
}

void solve() {
	rep (i, n) {
		if (rooted(i)) {
			puts ("SYMMETRIC");
			return ;
		}
	}
	puts ("NOT SYMMETRIC");
}

int main() {
	freopen ("C.out", "w", stdout);

	repcase {
		fprintf(stderr, "solving case %d / %d...\n", Case, t + Case - 1);
		printf ("Case #%d: ", Case++);

		input();

		solve();
	}
	return 0;
}