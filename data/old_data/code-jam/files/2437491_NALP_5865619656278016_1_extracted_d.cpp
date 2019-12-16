#define _CRT_SECURE_NO_DEPRECATE
#pragma comment(linker, "/STACK:500000000")

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <cmath>
#include <algorithm>
#include <utility>
#include <cassert>

#include <set>
#include <map>
#include <vector>
#include <string>
#include <queue>
#include <bitset>
#include <memory.h>

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

typedef long long int64;

#define forn(i, n) for(int i = 0; i < (int)(n); i++)
#define ford(i, n) for(int i = (int)(n) - 1; i >= 0; i--)
#define pb push_back
#define mp make_pair
#define y1 YYY1
#define all(a) (a).begin(), (a).end()
#define rall(a) (a).rbegin(), (a).rend()

template<typename T> inline T Abs(T x) { return (x >= 0) ? x : -x; }
template<typename T> inline T sqr(T x) { return x * x; }
template<typename T> inline string toStr(T x) { stringstream st; st << x; string s; st >> s; return s; }
template<typename T> inline int bit(T mask, int b) { return (b >= 0 && (mask & (T(1) << b)) != 0) ? 1 : 0; }

inline int nextInt() { int x; if (scanf("%d", &x) != 1) throw; return x; }
inline int64 nextInt64() { int64 x; if (scanf("%I64d", &x) != 1) throw; return x; }
inline double nextDouble() { double x; if (scanf("%lf", &x) != 1) throw; return x; }

const int INF = (int)1E9;
const int64 INF64 = (int64)1E18;
const long double EPS = 1E-9;
const long double PI = 3.1415926535897932384626433832795;

const int MAXN = 100100;

map<pair<int, pair<int, int> >, int> d1[MAXN];
vector<int> a[MAXN];
set<int> st[MAXN];
int n, k;

void norm(int &a, int &b, int &c) {
	if (a > c) swap(a, c);
	if (b > c) swap(b, c);
	if (a > b) swap(a, b);
}

int getd1(int x, int y1, int y2, int y3) {
	norm(y1, y2, y3);
	if (d1[x].count(mp(y1, mp(y2, y3)))) return d1[x][mp(y1, mp(y2, y3))];
	return 0;
}

void put(int v, int c1, int c2, int c3, int value) {
	norm(c1, c2, c3);
	d1[v][mp(c1, mp(c2, c3))] = value;
}

int go(int v, int c1, int c2, int c3) {
	if (v == n) return 0;
	if (c2 != -1 && d1[v].count(mp(c1, mp(c2, c3)))) return d1[v][mp(c1, mp(c2, c3))];

	int step = max(getd1(v, c1, -1, -1), max(getd1(v, c2, -1, -1), getd1(v, c3, -1, -1)));
	step = max(getd1(v, c1, c2, -1), max(getd1(v, c1, c3, -1), getd1(v, c2, c3, -1)));
	step = max(step, getd1(v, c1, c2, c3));
	
	if (step != 0) return step + go(v + step, c1, c2, c3);
	if (c1 != -1 && c2 != -1 && c3 != -1 && k == 3) return 0;
	if (c1 != -1 && c2 != -1 && k == 2) return 0;

	int ans = 0;
	forn(i, a[v].size()) {
		int nc1 = c1, nc2 = c2, nc3 = c3;
		int c = a[v][i];
		if (nc2 == -1)
			nc2 = c;
		else
			nc3 = c;

		step = getd1(v, c, -1, -1);
		int cur = step + go(v + step, nc1, nc2, nc3);
		ans = max(ans, cur);
		put(v, nc1, nc2, nc3, cur);
	}

	return ans;
}

void solve() {
	n = nextInt();
	int D = nextInt();
	k = nextInt();
	forn(i, n) {
		a[i].resize(D);
		forn(j, D)
			a[i][j] = nextInt();

		st[i] = set<int>(all(a[i]));
		d1[i].clear();
	}

	ford(i, n)
		forn(j, a[i].size()) {
			int c = a[i][j];
			put(i, c, -1, -1, 1 + getd1(i + 1, c, -1, -1));
		}

	int ll = 0, len = 0;
	ford(l, n)
		forn(jl, a[l].size()) {
			int value = go(l, a[l][jl], -1, -1);
			if (value >= len) {
				len = value;
				ll = l;
			}
		}

	cout << ll << " " << ll + len - 1 << endl;
}

int main() {
	freopen("input.txt", "rt", stdin);
	freopen("output.txt", "wt", stdout);

	srand((unsigned int)time(0));
	int tests = nextInt();
	forn(test, tests) {
		time_t start = clock();
		cerr << "Case #" << test + 1 << endl;
		cout << "Case #" << test + 1 << ": ";
		solve();
		cerr << "time is " << (0.0 + clock() - start) / CLOCKS_PER_SEC << endl;
	}

	return 0;
}
