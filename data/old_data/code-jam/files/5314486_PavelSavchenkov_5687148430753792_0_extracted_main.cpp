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

const int MAXN = 1e3 + 10;
const int INF = 1e9;

int N, C, M;
vi ticket[MAXN];

bool read() {
	if  (scanf("%d%d%d", &N, &C, &M) < 3) {
		return 0;
	}
	forn(i, C) {
		ticket[i].clear();
	}
	forn(it, M) {
		int pos, who;
		scanf("%d%d", &pos, &who);
		--pos;
		--who;
		ticket[who].pb(pos);
	}
	return 1;
}

bool was[MAXN][MAXN];
int used[MAXN];
int with[MAXN];
int dp[MAXN][MAXN];

void remin(int& x, int y) {
	if  (y == -1) {
		return;
	}
	if  (x == -1 || x > y) {
		x = y;
	}
}

int min_promotions(int y) {
	memset (was, 0, sizeof was);
	memset (used, 0, sizeof used);

	vii a;
	forn(i, C) {
		for (int to : ticket[i]) {
			a.pb(mp(to, i));
		}
	}

	sort(all(a));

	for (const auto& cur : a) {
		int to, id;
		tie(to, id) = cur;

		int best = -1;
		int best_used = INF;
		forn(i, y) {
			if  (was[i][id]) {
				continue;
			}
			if  (used[i] < best_used) {
				best_used = used[i];
				best = i;
			}
		}

		if  (best_used > to || best_used == N) {
			return INF;
		}
		was[best][id] = 1;
		++used[best];
	}

	// printf("y = %d\n", y);
	memset (with, 0, sizeof with);
	for (const auto& cur : a) {
		int to, id;
		tie(to, id) = cur;
		++with[to];
	}

	memset (dp, -1, sizeof dp);
	dp[0][0] = 0;
	forn(place, N) forn(low, M + 2) {
		const int cur_dp = dp[place][low];
		if  (cur_dp == -1) {
			continue;
		}
		// printf("dp[place=%d][low=%d] = %d\n", place, low, cur_dp);
		forn(to_low, with[place] + 1) {
			if (to_low > low) {
				break;
			}
			int up = with[place] - to_low;
			if  (up > y) {
				continue;
			}
			// printf("remin %d %d\n", place + 1, min(M, (low - to_low) + (y - up)));
			remin(dp[place + 1][min(M, (low - to_low) + (y - up))], cur_dp + to_low);
		}
	}

	int ans = INF;
	forn(was, M + 2) {
		remin(ans, dp[N][was]);
	}
	return ans;
}

void solve() {
	// min_promotions(2, false);
	// return;

	int L = 0;
	int R = M + 1;
	while (L != R - 1) {
		int mid = (L + R) >> 1;
		if  (min_promotions(mid) < INF) {
			R = mid;
		} else {
			L = mid;
		}
	}

	int y = R;
	printf("%d %d\n", y, min_promotions(y));
}

int main() {
#ifdef LOCAL
	freopen(FILE_NAME ".in", "r", stdin);
	freopen(FILE_NAME ".out", "w", stdout);
#endif

	int T;
	scanf("%d", &T);
	forn(t, T) {
		cerr << t << " / " << T << endl;
		
		assert(read());
		printf("Case #%d: ", t + 1);
		solve();
	}

#ifdef LOCAL
	cerr.precision(5);
	cerr << "Time: " << fixed << (double) clock() / CLOCKS_PER_SEC << endl;
#endif
	return 0;
}

