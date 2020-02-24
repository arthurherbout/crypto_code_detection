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

#define pb push_back
#define fst first
#define snd second
#define mp make_pair 
#define sz(C) ((int) (C).size())
#define forn(i, n) for (int i = 0; i < (int) n; ++i)
#define ford(i, n) for (int i = ((int) n) - 1; i >= 0; --i)
#define y1 gftxdtrtfhyjfctrxujkvbhyjice
#define y0 ehfoiuvhefroerferjhfjkehfjke
#define left sdhfsjkshdjkfsdfgkqqweqweh
#define right yytrwtretywretwreytwreytwr
#define next jskdfksdhfjkdsjksdjkgf
#define prev koeuigrihjdkjdfj
#define hash kjfdkljkdhgjdkfhgurehg
#define all(C) begin(C), end(C)

typedef long long ll;
typedef unsigned long long ull;
typedef unsigned int uint;
typedef pair<int,int> pii;
typedef pair<ll, ll> pll;
typedef vector<ll> vll;
typedef vector<int> vi;
typedef vector<vector <int> > vvi;
typedef vector<pii> vii;
typedef long double ld;
typedef complex<double> cd;
typedef vector<cd> vcd;

#define FILE_NAME "a"

const int MAXN = 200 + 10;

ld dp[MAXN][MAXN];
int n, k;
ld p[MAXN];

bool read() {
	if  (scanf("%d%d", &n, &k) < 2) {
		return false;
	}
	forn(i, n) {
		double x;
		scanf("%lf", &x);
		p[i] = x;
	}
	return true;
}

bool bit(int mask, int i) {
	return (mask >> i) & 1;
}

ld brut_solve() {
	ld mx = 0;
	forn(mask, 1 << n) {
		if  (__builtin_popcount(mask) != k) {
			continue;
		}

		vector<ld> ps;

		forn(i, n) {
			if  (bit(mask, i)) {
				ps.pb(p[i]);
			}
		}

		memset (dp, 0, sizeof dp);
		dp[0][0] = 1.0;
		forn(i, k) {
			forn(yes, i + 1) {
				ld d = dp[i][yes];
				dp[i + 1][yes + 1] += d * ps[i];
				dp[i + 1][yes] += d * (1.0 - ps[i]);
			}
		}
		mx = max(mx, dp[k][k / 2]);
	}
	return mx;
}

ld D[MAXN][MAXN][MAXN];

ld solve() {
	sort(p, p + n);
	// forn(i, n) {
	// 	printf("%.5f ", (double) p[i]);
	// }
	// puts("");
	ld mx = 0;

	forn(i, n + 1) forn(j, n + 1) {
		if  (i + j != k) {
			continue;
		}
		vector<ld> ps;
		forn(it, i) {
			ps.pb(p[it]);
		}
		forn(it, j) {
			ps.pb(p[n - 1 - it]);
		}

		memset (dp, 0, sizeof dp);
		dp[0][0] = 1.0;
		forn(i, k) {
			forn(yes, i + 1) {
				ld d = dp[i][yes];
				dp[i + 1][yes + 1] += d * ps[i];
				dp[i + 1][yes] += d * (1.0 - ps[i]);
			}
		}
		mx = max(mx, dp[k][k / 2]);
	}

	// forn(i, n) for (int j = i; j < n; ++j) {
	// 	if  (j - i + 1 != k) {
	// 		continue;
	// 	}
		
	// }

	return mx;

	memset (D, 0, sizeof D);
	D[0][0][0] = 1.0;
	forn(i, n) {
		forn(j, n) {
			if  (i + j >= n) {
				continue;
			}  
			forn(yes, n + 1) {
				ld d = D[i][j][yes];
				
				if  (!j)
					D[i + 1][j][yes] += d * (1.0 - p[i]);
				D[i][j + 1][yes] += d * (1.0 - p[n - 1 - j]);

				if  (!j)
					D[i + 1][j][yes + 1] += d * p[i];
				D[i][j + 1][yes + 1] += d * p[n - 1 - j]; 
			}
		}
	}

	forn(i, n + 1) forn(j, n + 1) {
		// if  (i + j != k) {
		// 	continue;
		// }
		mx = max(mx, D[i][j][k / 2]);
		printf("D[%d][%d][%d] = %.10f\n", i, j, k / 2, (double) D[i][j][k / 2]);
	}

	return mx;
}

int main() {
#ifdef LOCAL
	freopen(FILE_NAME ".in", "r", stdin);
	freopen(FILE_NAME ".out", "w", stdout);
#endif


	int T;
	scanf("%d\n", &T);
	forn(t, T) {
		assert(read());
		// ld brut = brut_solve();
		// ld ans = solve();
		// printf("brut = %.10f, ans = %.10f\n", (double) brut, (double) ans);
		// assert(fabs(brut - ans) < 1e-7);
		printf("Case #%d: %.10f\n", t + 1, (double) solve());
		cerr << t << endl;
	}

#ifdef LOCAL
	// printf("%.2f\n", (double) clock() / CLOCKS_PER_SEC);
#endif
	return 0;
}
