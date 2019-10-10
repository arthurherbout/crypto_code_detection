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

using namespace std;

#define pb push_back
#define f first
#define s second
#define mp make_pair 
#define sz(C) ((int) (C).size())
#define forn(i, n) for (int i = 0; i < (int) n; ++i)
#define ford(i, n) for (int i = ((int) n) - 1; i >= 0; --i)
#define foran(i, a, n) for (int i = (int) a; i < (int) n; ++i)
#define y1 gftxdtrtfhyjfctrxujkvbhyjice
#define y0 ehfoiuvhefroerferjhfjkehfjke
#define left sdhfsjkshdjkfsdfgkqqweqweh
#define right yytrwtretywretwreytwreytwr
#define all(C) (C).begin(), (C).end()

template <typename T>
inline T sqr( T x ) {
	return x * x;
}

typedef long long ll;
typedef unsigned long long ull;
typedef unsigned int uint;
typedef pair <int,int> pii;
typedef pair <ll, ll> pll;
typedef vector <ll> vll;
typedef vector <int> vi;
typedef vector <vector <int> > vvi;
typedef vector <pii> vii;

const double EPS = 1e-9;
const int MAXN = 50 + 10;

bool g[MAXN][MAXN];
int n, m;
string s[MAXN];
bool was[MAXN];

ll dfs( ll Mask, int v ) {
	was[v] = true;
	ll mask = 1ll << v;

	forn(to, n)
		if  (g[v][to] && !was[to] && ((Mask >> to) & 1ll)) {
			mask |= dfs(Mask, to);
		}

	return mask;
}

string go( ll mask, ll mask_start, ll mask_end );

string go_root( int root, ll mask, ll mask_end ) {
//	printf("go root=%d %I64d %I64d\n", root + 1, mask, mask_end);

	if  (!mask_end) {
		return "";
	}

	mask_end &= mask;
	memset (was, false, sizeof was);
	ll mask_left = 0;
	ll mask_right = 0;
	was[root] = true;

	forn(to, n)
		if  (g[to][root] && !was[to] && ((mask >> to) & 1ll)) {
			if  (!mask_left) {
				mask_left = dfs(mask, to);
			} else if  (!mask_right) {
				mask_right = dfs(mask, to);
			} else {
//				printf("bad at %d\n", root);
				return s[root];
			}	
		}

	if  (!mask_left) {
//		printf("root = %d\n", root + 1);
		assert(__builtin_popcount(mask) == 1);
		return s[root];
	}

//	printf("left %I64d right %I64d\n", mask_left, mask_right);

	ll new_mask_end = 0;
	forn(to, n)
		if  ((mask_left >> to) & 1ll && g[root][to]) {
			new_mask_end |= 1ll << to;
		}	

	ll new_mask_end2 = 0;
	forn(to, n)
		if  ((mask_right >> to) & 1ll && g[root][to]) {
			new_mask_end2 |= 1ll << to;
		}	

	if  (!mask_right) {
		// return to root
		string res = "";
		if  ((mask_end >> root) & 1ll) {
			res = go(mask_left, new_mask_end, new_mask_end);
		}

		// do not return to root
		string res2 = go(mask_left, new_mask_end, mask_end);

		if  (res.empty() || res2 < res) {
			res = res2;
		}

		if  (res.empty()) {
			return res;
		}

		return s[root] + res;
	}

	// left and right
	
	string ans = "";
	// 1)
	// to left
	string res = "";
	res = go(mask_left, new_mask_end, new_mask_end);
	// to right
	string res2 = go(mask_right, new_mask_end2, mask_end);
	if  (!res.empty() && !res2.empty())
		ans = res + res2;

	// 2)
	// to right
	res = "";
	res = go(mask_right, new_mask_end2, new_mask_end2);
	// to left
	res2 = go(mask_left, new_mask_end, mask_end);

	if (!res.empty() && !res2.empty())
		ans = min(res, ans);

	if (ans.empty()) {
		return ans;
	}

	return s[root] + ans;
}

string go( ll mask, ll mask_start, ll mask_end ) {
//	printf("%I64d %I64d %I64d\n", mask, mask_start, mask_end);

	string res = "";

	forn(i, n)
		if  ((mask_start >> i) & 1ll) {
			string cur = go_root(i, mask, mask_end);
			if  ((res.empty() || cur < res) && !cur.empty()) {
				res = cur;
//				return res;
			}
		}

	return res;
}

int main() {
	freopen("in.txt", "r", stdin);
	freopen("out.txt", "w", stdout);

	int T;
	scanf("%d", &T);
	forn(it, T) {
		scanf("%d%d\n", &n, &m);

		forn(i, n) {
			cin >> s[i];
		}

		memset (g, false, sizeof g);
		forn(i, m) {
			int a, b;
			scanf("%d%d", &a, &b);
			--a;
			--b;
			g[a][b] = g[b][a] = true;
//			printf("%d %d\n", a + 1, b + 1);
		}

		printf("Case #%d: %s\n", it + 1, go((1ll << n) - 1ll, (1ll << n) - 1ll, (1ll << n) - 1ll).c_str());
	}

	return 0;
}
