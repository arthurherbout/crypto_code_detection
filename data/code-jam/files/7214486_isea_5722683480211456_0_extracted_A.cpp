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

const int MAXN = 2000 + 10;

// int even[MAXN], odd[MAXN];
// int even_r[MAXN], odd_r[MAXN];

char mat[MAXN][MAXN];
int n;

void input() {
	scanf ("%d", &n);
	rep (i, 2 * n) {
		scanf ("%s", mat[i]);
	}
}

bool check(string s1, string s2) {
	// cout << s1 << ' ' << s2 << endl;
	rep (i, 2 * n) {
		if (s1[i] == s2[i]) {
			return false;
		}
	}
	return true;
}

int get_ans() {
	set<string> s;
	rep (i, 2 * n) {
		string c = "";
		rep (j, 2 * n) {
			c += mat[i][j];
		}
		s.insert(c);
	}
	if (sz(s) != 2) {
		return -1;
	}
	set<string>::iterator it = s.begin();
	set<string>::iterator jt = it++;

	if (!check(*it, *jt)) {
		return -1;
	}

	s.clear();
	rep (i, 2 * n) {
		string c = "";
		rep (j, 2 * n) {
			c += mat[j][i];
		}
		s.insert(c);
	}
	if (sz(s) != 2) {
		return -1;
	}
	
	it = s.begin();
	jt = it++;

	if (!check(*it, *jt)) {
		return -1;
	}

	int odd = 0, even = 0;
	rep (i, 2 * n) {
		if (mat[i][0] == '1') {
			if (i & 1) {
				odd += 1;
			} else {
				even += 1;
			}
		}
	}
	if (odd + even != n) {
		return -1;
	}
	int res = odd;
	odd = 0, even = 0;
	rep (i, 2 * n) {
		if (mat[0][i] == '1') {
			if (i & 1) {
				odd += 1;
			} else {
				even += 1;
			}
		}
	}
	if (odd + even != n) {
		return -1;
	}
	return 2 * n - res - odd;
}

void solve() {
	int res = get_ans();
	if (res == -1) {
		puts ("IMPOSSIBLE");
	} else {
		printf ("%d\n", res);
	}
}

int main() {
	freopen ("A.out", "w", stdout);

	repcase {
		fprintf(stderr, "solving case %d / %d...\n", Case, t + Case - 1);
		printf ("Case #%d: ", Case++);

		input();

		solve();
	}
	return 0;
}