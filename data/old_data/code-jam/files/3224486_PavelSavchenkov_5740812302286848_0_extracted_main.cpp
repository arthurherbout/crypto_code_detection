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


const int MAXN = 1e3 + 10;

struct Point {
	int x, y, z;

	Point(int x, int y, int z) : x(x), y(y), z(z) {}

	Point() {

	}
	
	bool read() {
		if  (scanf("%d%d%d", &x, &y, &z) < 3) {
			return false;
		}
		return true;
	}

	ld len() const {
		return sqrtl(x * x + y * y + z * z);
	}

	Point operator - (const Point& p) const {
		return Point(x - p.x, y - p.y, z - p.z);
	}
};

Point p[MAXN];
Point v[MAXN];
int n, s;

bool read() {
	if  (scanf("%d%d", &n, &s) < 2) {
		return false;
	}
	forn(i, n) {
		if  (!p[i].read()) {
			return false;
		}
		if  (!v[i].read()) {
			return false;
		}
	}
	return true;
}

bool used[MAXN];
ld dist[MAXN];
const ld INF = 1e20;

ld solve() {
	forn(i, n) {
		dist[i] = INF;
		used[i] = false;
	}

	dist[0] = 0;
	while (1) {
		int min_i = -1;
		forn(i, n) {
			if  (!used[i] && (min_i == -1 || dist[i] < dist[min_i])) {
				min_i = i;
			}
		}

		if  (min_i == -1) {
			break;
		}
		
		used[min_i] = true;
		forn(i, n) {
			dist[i] = min(dist[i], max(dist[min_i], (p[i] - p[min_i]).len()));
		}
	}

	return dist[1];
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
		printf("Case #%d: %.15f\n", t + 1, (double) solve());
	}

#ifdef LOCAL
	// printf("Time: %.2f\n", (double) clock() / CLOCKS_PER_SEC);
#endif
	return 0;
}
