#include <cstdio>
#include <iostream>
#include <algorithm>
#include <cstring>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <utility>
#include <cstdlib>
#include <memory>
#include <queue>
#include <cassert>
#include <cmath>
#include <ctime>
#include <complex>
#include <bitset>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <numeric>

using namespace std;

#define ws ws_____________________
#define y1 y1_____________________
#define y0 y0_____________________
#define left left_________________
#define right right_______________
#define next next_________________
#define prev prev_________________
#define hash hash_________________

#define pb push_back
#define fst first
#define snd second
#define mp make_pair 
#define sz(C) ((int) (C).size())
#define forn(i, n) for (int i = 0; i < int(n); ++i)
#define ford(i, n) for (int i = int(n) - 1; i >= 0; --i)
#define all(C) begin(C), end(C)

typedef long long ll;
typedef unsigned long long ull;
typedef unsigned int uint;
typedef pair<int,int> pii;
typedef pair<ll, ll> pll;
typedef vector<ll> vll;
typedef vector<int> vi;
typedef vector<vi> vvi;
typedef vector<pii> vii;
typedef long double ld;
typedef complex<double> cd;

#define FILE_NAME "a"

const int MAXN = 100 + 10;
const ld EPS = 1e-10;

int n, q;
ll d[MAXN][MAXN];
ll e[MAXN];
ll s[MAXN];

bool read() {
	if  (scanf("%d%d", &n, &q) < 2) {
		return 0;
	}
	forn(i, n) {
		scanf("%lld%lld", &e[i], &s[i]);
	}
	forn(i, n) forn(j, n) {
		scanf("%lld", &d[i][j]);
	}
	return 1;
}

ld ans[MAXN][MAXN];

void solve() {
	forn(i, n) {
		d[i][i] = 0;
	}
	forn(i, n) forn(j, n) {
		if  (d[i][j] == -1) {
			d[i][j] = ll(1e13);
		}
	}
	forn(k, n) {
		forn(i, n) forn(j, n) {
			d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
		}
	}

	forn(i, n) forn(j, n) {
		if  (e[i] < d[i][j]) {
			ans[i][j] = 1e20;
			continue;
		}
		ans[i][j] = d[i][j] * 1.0 / s[i];
	}

	forn(k, n) {
		forn(i, n) forn(j, n) {
			if  (ans[i][k] < 1e15 && ans[k][j] < 1e15) {
				ld nw = ans[i][k] + ans[k][j];
				if  (nw + EPS < ans[i][j]) {
					ans[i][j] = nw;
				}
			}
		}
	}

	forn(it, q) {
		int a, b;
		scanf("%d%d", &a, &b);
		--a;
		--b;
		printf("%.10f ", (double) ans[a][b]);
	}
	puts("");
}

int main() {
#ifdef LOCAL
	freopen(FILE_NAME ".in", "r", stdin);
	freopen(FILE_NAME ".out", "w", stdout);
#endif

	int T;
	scanf("%d", &T);
	forn(t, T) {
		assert(read());
		printf("Case #%d: ", t + 1);
		solve();

		cerr << t + 1 << endl;
	}

#ifdef LOCAL
	cerr.precision(5);
	cerr << "Time: " << fixed << (double) clock() / CLOCKS_PER_SEC << endl;
#endif
	return 0;
}

