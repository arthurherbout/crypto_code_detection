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


const int MAXN = 4 + 10;

char a[MAXN][MAXN];
int n;
char b[MAXN][MAXN];

bool read() {
	if  (scanf("%d\n", &n) < 1) {
		return false;
	}
	forn(i, n) {
		gets(a[i]);
	}
	return true;
}

bool bit(int mask, int i) {
	return (mask >> i) & 1;
}

int p[MAXN];
bool was[MAXN];

bool rec(int i) {
	if  (i == n) {
		return true;
	}
	int who = p[i];
	bool bad = true;
	forn(j, n) {
		if  (b[who][j] == '1') {
			if  (was[j]) {
				continue;
			}
			was[j] = true;
			if  (!rec(i + 1)) {
				return false;
			}
			bad = false;
			was[j] = false;
		}
	}
	return !bad;
}

bool check() {
	// forn(i, n) {
	// 	puts(b[i]);
	// }
	// puts("");
	iota(p, p + n, 0);
	do {
		memset (was, false, sizeof was);
		if  (!rec(0)) {
			return false;
		}
	} while (next_permutation(p, p + n));
	return true;
}

int solve() {
	int ans = n * n;
	forn(mask, 1 << (n * n)) {
		bool bad = false;
		int cost = 0;
		forn(i, n) forn(j, n) {
			b[i][j] = a[i][j];
			int k = i * n + j;
			if  (bit(mask, k)) {
				if  (a[i][j] == '1') {
					bad = true;
				}
				++cost;
				b[i][j] = '1';
			}
		}
		if  (bad) {
			continue;
		}
		if  (cost >= ans) {
			continue;
		}
		if  (check()) {
			ans = cost;
		}
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
		printf("Case #%d: %d\n", t + 1, solve());
		cerr << t << endl;
	} 

#ifdef LOCAL
	// printf("%.2f\n", (double) clock() / CLOCKS_PER_SEC);
#endif
	return 0;
}
