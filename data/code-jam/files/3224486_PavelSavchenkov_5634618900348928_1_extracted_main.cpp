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


const int MAXN = 2e4 + 10;
const int INF = 1e9;

char s[MAXN];
int n;
int nxt[MAXN][2];

bool read() {
	gets(s);
	n = strlen(s);
	return n > 0;
}

int dp[MAXN][MAXN][2];

// int d[MAXN][MAXN];

// int solve_brut() {
// 	memset (d, 0, sizeof d);
// 	for (int len = 2; len <= n; len += 2) {
// 		forn(l, n) {
// 			int r = l + len - 1;
// 			if  (r >= n) {
// 				break;
// 			}
			
// 			for (int i = l + 1; i <= r; i += 2) {
// 				int score = 5 + (s[l] == s[i]) * 5;
// 				if  (l + 1 <= i - 1) {
// 					score += d[l + 1][i - 1];
// 				}
// 				if  (i + 1 <= r) {
// 					score += d[i + 1][r];
// 				}
// 				d[l][r] = max(d[l][r], score);
// 			}
// 		}
// 	}
// 	return d[0][n - 1];
// }

// int solve() {
// 	forn(i, n) {
// 		forn(c, 2) {
// 			nxt[i][c] = n;
// 			for (int j = i + 1; j < n; ++j) {
// 				if  ((s[j] == 'C') == c && (j - i + 1) % 2 == 0) {
// 					nxt[i][c] = j;
// 					break;
// 				}
// 			}
// 			// printf("nxt[%d][%d] = %d\n", i, c, nxt[i][c]);
// 		}
// 	}

// 	memset (d, 0, sizeof d);
// 	for (int len = 2; len <= n; len += 2) {
// 		forn(l, n) {
// 			int r = l + len - 1;
// 			if  (r >= n) {
// 				break;
// 			}

// 			forn(c, 2) {
// 				int i = nxt[l][c];
// 				if  (i > r) {
// 					continue;
// 				}
// 				int score = 5 + (s[l] == s[i]) * 5;
// 				if  (l + 1 <= i - 1) {
// 					score += d[l + 1][i - 1];
// 				}
// 				if  (i + 1 <= r) {
// 					score += d[i + 1][r];
// 				}
// 				d[l][r] = max(d[l][r], score);
// 			}

// 			// for (int i = l + 1; i <= r; i += 2) {
// 			// 	int score = 5 + (s[l] == s[i]) * 5;
// 			// 	if  (l + 1 <= i - 1) {
// 			// 		score += d[l + 1][i - 1];
// 			// 	}
// 			// 	if  (i + 1 <= r) {
// 			// 		score += d[i + 1][r];
// 			// 	}
// 			// 	d[l][r] = max(d[l][r], score);
// 			// }
// 		}
// 	}
// 	return d[0][n - 1];
// }

// int solve_new() {
// 	forn(i, n + 1) forn(j, n + 1) forn(c, 2) dp[i][j][c] = -1;

// 	dp[0][0][0] = 0;

// 	forn(i, n) forn(len, i + 1) forn(c, 2) {
// 		int cur_dp = dp[i][len][c];
// 		if  (cur_dp == -1) {
// 			continue;
// 		}

// 		int cur = s[i] == 'C';
// 		if  (len > 0 && cur == c) {
// 			dp[i + 1][len - 1][c ^ 1] = max(dp[i + 1][len - 1][c ^ 1], cur_dp + 10);
// 		}
// 		if  (len > 0 && cur != c) {
// 			dp[i + 1][len - 1][cur] = max(dp[i + 1][len - 1][cur], cur_dp + 5);
// 		}
// 		dp[i + 1][len + 1][cur] = max(dp[i + 1][len + 1][cur], cur_dp);
// 	}

// 	int ans = 0;
// 	forn(len, n + 1) forn(c, 2) {
// 		int cur_dp = dp[n][len][c];
// 		if  (cur_dp == -1) {
// 			continue;
// 		}
// 		ans = max(ans, cur_dp + (len / 2) * 5);
// 	}

// 	return ans;
// }

int solve_old() {
	forn(i, n + 1) forn(j, n + 1) forn(c, 2) dp[i][j][c] = -INF;

	dp[0][0][0] = 0;

	forn(i, n) forn(len, i + 1) forn(c, 2) {
		int cur_dp = dp[i][len][c];
		if  (cur_dp <= -INF) {
			continue;
		}

		int cur = s[i] == 'C';
		if  (len > 0 && cur == c) {
			dp[i + 1][len - 1][c ^ 1] = max(dp[i + 1][len - 1][c ^ 1], cur_dp + 10);
		} else {
			dp[i + 1][len + 1][cur] = max(dp[i + 1][len + 1][cur], cur_dp);
		}
	}

	int ans = 0;
	forn(len, n + 1) forn(c, 2) {
		int cur_dp = dp[n][len][c];
		if  (cur_dp <= -INF) {
			continue;
		}
		ans = max(ans, cur_dp + (len / 2) * 5);
	}

	return ans;
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
		// cerr << solve_old() << " " << solve_brut() << std::endl;
		// cerr << solve_brut() << " " << solve() << std::endl;
		// assert(solve_brut() == solve());
		// assert(solve_new() == solve_old());
		// printf("old = %d, now = %d\n", solve_new(), solve());
		// assert(solve_new() <= solve());
		printf("Case #%d: %d\n", t + 1, solve_old());
		cerr << t << endl;
	}

#ifdef LOCAL
	// printf("Time: %.2f\n", (double) clock() / CLOCKS_PER_SEC);
#endif
	return 0;
}
