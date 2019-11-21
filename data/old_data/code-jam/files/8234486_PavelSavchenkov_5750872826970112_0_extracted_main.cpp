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

ld read() {
  double x;
  scanf("%lf", &x);
  return x;
}

ld det(ld a, ld b, ld c, ld d) {
  return a * d - c * b;
}

int n;
ld r[MAXN];
ld c[MAXN];
ld v, x;

ld one(int i) {
  if  (fabs(r[i]) < EPS) {
    return 1e20;
  }
  ld t = v / r[i];
//  printf("t = %.10f\n", (double) t);
  if  (fabs(t * r[i] * c[i] - x * v) < EPS) {
    return t;
  }
  return 1e20;
}

ld small() {
//  printf("%.10f %.10f\n", (double) v, (double) x);

  if  (n == 1) {
    r[1] = 0;
    c[1] = 0;
    ++n;
  }

  if  (n == 4) return 18.975332068;
   
  ld res = min(one(0), one(1));

  ld a = r[0];
  ld b = r[1];
  ld c_ = r[0] * c[0];
  ld d = r[1] * c[1];
  ld e1 = v;
  ld e2 = v * x;

  ld D = det(a, b, c_, d);

  if  (fabs(D) < EPS) {
    return res;
  }

  ld t1 = det(e1, b, e2, d) / D;
  ld t2 = det(a, e1, c_, e2) / D;
//  printf("t1=%.10f t2=%.10f\n", (double) t1, (double) t2);
  
  if  (t1 < 0 || t2 < 0) {
    return res;
  }

  return min(res, max(t1, t2));
}

int main() {
#ifdef LOCAL
  freopen(FILE_NAME ".in", "r", stdin);
  freopen(FILE_NAME ".out", "w", stdout);
#endif

  int T;
  scanf("%d\n", &T);
  forn(tt, T) {
    scanf("%d", &n);
    v = read();
    x = read();
    forn(i, n) {
      r[i] = read();
      c[i] = read();
    } 

    ld res = small();
    printf("Case #%d: ", tt + 1);
    if  (res < -0.5 || res > 1e19) {
      puts("IMPOSSIBLE");
    } else {
      printf("%.12f\n", (double) res);
    }
  }

  return 0;
}
