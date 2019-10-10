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
typedef pair <int,int> pii;
typedef pair <ll, ll> pll;
typedef vector <ll> vll;
typedef vector <int> vi;
typedef vector <vector <int> > vvi;
typedef vector <pii> vii;
typedef long double ld;
typedef complex<double> cd;
typedef vector<cd> vcd;

#define FILE_NAME ""

const ld EPS = 1e-9;
const int MAXN = 100 + 10;

const int di[4] = { 1, -1, 0, 0 };
const int dj[4] = { 0, 0, -1, 1 };

int dir(char c) {
  if  (c == '^') {
    return 1;
  }
  if  (c == '<') {
    return 2;
  }
  if  (c == '>') {
    return 3;
  }
  assert(c == 'v');
  return 0;
}

char get_ch(int id) {
  return "v^<>"[id];
}

int n, m;
char a[MAXN][MAXN];

bool on(int i, int j) {
  return 0 <= i && i < n && 0 <= j && j < m;
}

pii get_bad() {
  forn(i, n) forn(j, m) {
    if  (a[i][j] == '.') {
      continue;
    }

    int id = dir(a[i][j]);

    bool ok = false;
    for (int ii = i + di[id], jj = j + dj[id]; on(ii, jj); ii += di[id], jj += dj[id]) {
      if  (a[ii][jj] != '.') {
        ok = true;
      } 
    } 

    if  (ok) {
      continue;
    }

    return mp(i, j);
  }

  return mp(-1, -1);
}

bool make_ok(int i, int j) {
  forn(id, 4) {
    bool ok = false;
    for (int ii = i + di[id], jj = j + dj[id]; on(ii, jj); ii += di[id], jj += dj[id]) {
      if  (a[ii][jj] != '.') {
        ok = true;
      }
    }

    if  (ok) {
      a[i][j] = get_ch(id);
      return true;
    }
  }

  return false;
}

int solve() {
  int res = 0;
  while (1) {
    int i, j;
    tie(i, j) = get_bad();

    if  (i == -1) {
      return res;
    }

    if  (!make_ok(i, j)) {
      return -1;
    }

    ++res;
  }

  return res;
}

int main() {
#ifdef LOCAL
  freopen(FILE_NAME ".in", "r", stdin);
  freopen(FILE_NAME ".out", "w", stdout);
#endif

  int T;
  scanf("%d\n", &T);
  forn(tt, T) {
    scanf("%d%d\n", &n, &m);
    forn(i, n) gets(a[i]);

    int res = solve();
    if  (res >= 0) {
      printf("Case #%d: %d\n", tt + 1, res);
    } else {
      printf("Case #%d: IMPOSSIBLE\n", tt + 1);
    } 

//    forn(i, n) puts(a[i]);
  }

  return 0;
}
