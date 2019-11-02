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
const int A = 26;

string p;
ld prob[A];
int k, l, s;

string read_string() {
  static char buf[100];
  gets(buf);
  return string(buf);
}

bool read() {
  if  (scanf("%d%d%d\n", &k, &l, &s) < 2) {
    return false;
  }
//  s = 100;

  string t = read_string();
  forn(c, A) {
    prob[c] = 0.0;
  } 
  for (char c : t) {
    prob[c - 'A'] += 1.0 / sz(t);
  }
  p = read_string();
/*
  cout << p << endl;
  forn(c, A) {
    if  (prob[c] > 1e-9) {
      printf("%c -> %.10f\n", 'A' + c, (double) prob[c]);
    }
  }
*/
  return true;
}

const int MAXN = 1e2 + 10;

ld dp[MAXN][MAXN][MAXN];
int pref[MAXN];
int go[A][MAXN];

ld solve() {
  pref[0] = 0;
  for (int i = 1; i < sz(p); ++i) {
    int j = pref[i - 1];
    while (j > 0 && p[i] != p[j]) j = pref[j - 1];
    j += p[i] == p[j];
    pref[i] = j;
  }

  for (char c : p) {
    if  (prob[c - 'A'] < 1e-9) {
      return 0.0;
    }
  }

  int add = sz(p) - pref[sz(p) - 1];
  int have = 1 + (s - sz(p)) / add;

//  printf("have = %d\n", have);

  // go
  forn(c, A) {
    forn(suff, sz(p)) {
      if  (suff == 0) {
        go[c][suff] = c == p[0] - 'A';
      } else {
        go[c][suff] = c == p[suff] - 'A' ? suff + 1 : go[c][pref[suff - 1]];
      }
      if  (go[c][suff] == sz(p)) {
        go[c][suff] = pref[sz(p) - 1];
      }
    }
  }

  forn(i, s + 1) forn(suff, sz(p) + 1) forn(cnt, s + 1) dp[i][suff][cnt] = 0.0;
  dp[0][0][0] = 1.0;

  forn(i, s + 1) {
    forn(suff, sz(p) + 1) {
      forn(cnt, s + 1) {
//        if  (dp[i][suff][cnt] > EPS) printf("i=%d, suff=%d, cnt=%d -> %.5f\n", i, suff, cnt, (double) dp[i][suff][cnt]);
        forn(c, A) {
          int nsuff = go[c][suff];
          int ncnt = cnt + (suff + 1 == sz(p) && c == p.back() - 'A');
          dp[i + 1][nsuff][ncnt] += dp[i][suff][cnt] * prob[c];  
        }
      }
    }
  }

  ld ans = 0;
  forn(suff, sz(p)) forn(cnt, s + 1) {
    ans += dp[s][suff][cnt] * cnt;
  }

//  printf("ans = %.10f\n", (double) ans);

  return have - ans;
}

int main() {
#ifdef LOCAL
  freopen(FILE_NAME ".in", "r", stdin);
  freopen(FILE_NAME ".out", "w", stdout);
#endif

  int T;
  scanf("%d\n", &T);
  forn(tt, T) {
    assert(read());
    printf("Case #%d: %.20f\n", tt + 1, (double) solve());
    cerr << tt << endl;
  //  break;
  }

  return 0;
}
