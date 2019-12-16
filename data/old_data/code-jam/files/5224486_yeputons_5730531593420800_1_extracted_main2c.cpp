#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <cassert>
#include <algorithm>
#include <string>
#include <vector>
#include <deque>
#include <map>
#include <set>

using namespace std;

#define eprintf(...) fprintf(stderr, __VA_ARGS__)
#define pb push_back
#define mp make_pair
#define sz(x) ((int)(x).size())
#define forn(i, n) for (int i = 0; i < (n); i++)

typedef long long ll;
typedef long double ld;
typedef vector<ll> vll;
typedef vector<int> vi;
typedef vector<vi> vvi;
typedef vector<bool> vb;
typedef vector<vb> vvb;
typedef pair<int, int> pii;

int sgn(ll x) { return x < 0 ? -1 : !!x; }
struct pt {
  ll x, y;
  pt() : x(0), y(0) {}
  pt(ll _x, ll _y) : x(_x), y(_y) {}
  pt operator-(const pt &p2) const { return pt(x - p2.x, y - p2.y); }
  int operator*(const pt &p2) const { return sgn(x * p2.y - y * p2.x); }
};

template<int K>
bool good(const pt &a, const pt &b, const pt &c) {
  const pt &v1 = b - a, v2 = c - b;
  return v1 * v2 * K > 0;
}

void solve() {
  int n, f1, f;
  assert(scanf("%d %d.%d", &n, &f1, &f) == 3);
  const int DENOM = int(1e6);
  f += f1 * DENOM;
  eprintf("n=%d, f=%d\n", n, f);

  static char buf[int(1e6) + 1];
  scanf("%s", buf);
  assert((int)strlen(buf) == n);

  vector<pt> pts(n + 1);
  {
    ll sum = 0;
    pts[0] = pt(0, 0);
    forn (i, n) {
      sum += buf[i] == '1';
      pts[i + 1] = pt(i + 1, sum * DENOM - (i + 1) * f);
    }
  }
  n++;

//  forn (i, n)
//    eprintf("%I64d %I64d\n", pts[i].x, pts[i].y);

  pt ans(0, 1);
  int ansl = -1;

  vi up(n, n), dn(n, n);

  map<ll, int> coords;
  int ops = 0;
  for (int i = n - 1; i >= 0; i--) {
    bool horRight = false;
    {
      auto it = coords.lower_bound(pts[i].y);
      if (it != coords.end()) {
        auto it2 = it;
        if (it2->first == pts[i].y) {
          horRight = true;
          it2++;
        }
        if (it2 != coords.end()) {
          up[i] = it2->second;
        }
      }
      if (it != coords.begin()) {
        auto it2 = it; it2--;
        dn[i] = it2->second;
      }
    }
    coords[pts[i].y] = i;

    #ifdef DEBUG2
    {
      ll upy = ll(1e18), dny = -ll(1e18);
      int upi = n, dni = n;
      bool hri = false;
      for (int i2 = i + 1; i2 < n; i2++) {
        if (pts[i].y < pts[i2].y) {
          if (pts[i2].y < upy)
            upi = i2, upy = pts[i2].y;
        } else if (pts[i].y > pts[i2].y) {
          if (pts[i2].y > dny)
            dni = i2, dny = pts[i2].y;
        } else {
          hri = true;
        }
      }
      assert(up[i] == upi);
      assert(dn[i] == dni);
      assert(horRight == hri);
    }
    #endif
    if (horRight) {
      ans = pt(1, 0);
      ansl = i;
    }
/*  // ^_^ no need
   if (up[i] < n) {
      while (up[up[i]] < n && !good<+1>(pts[i], pts[up[i]], pts[up[up[i]]])) {
        up[i] = up[up[i]];
        ops++;
      }
    }
    if (dn[i] < n) {
      while (dn[dn[i]] < n && !good<-1>(pts[i], pts[dn[i]], pts[dn[dn[i]]])) {
        dn[i] = dn[dn[i]];
        ops++;
      }
    }*/
//    eprintf("i=%d; %d %d %d\n", i, up[i], !!horRight[i], dn[i]);
    if (up[i] < n) {
      pt cans = pts[up[i]] - pts[i];
      if (cans * ans >= 0) {
        ans = cans;
        ansl = i;
      }
    }
    if (dn[i] < n) {
      pt cans = pts[dn[i]] - pts[i];
      cans.y = -cans.y;
      if (cans * ans >= 0) {
        ans = cans;
        ansl = i;
      }
    }
  }
  eprintf("ops=%d\n", ops);
  eprintf("ans=%I64d %I64d\n", ans.x, ans.y);
  printf("%d\n", ansl);
}

bool endsWith(string a, string b) {
  return a.length() >= b.length() && string(a, a.length() - b.length()) == b;
}

int main(int argc, char *argv[]) {
  {
    string fn = "";
    if (argc >= 2) fn = argv[1];
    if (endsWith(fn, ".in")) fn = string(fn, 0, fn.length() - 3);
    assert(freopen((fn + ".in").c_str(), "r", stdin));
    freopen((fn + ".out").c_str(), "w", stdout);
  }

  int MARK;
  assert(scanf("%d", &MARK) >= 1);

  int START = 1, TC = MARK;
  if (MARK < 0) {
    START = TC = -MARK;
  }
  for (int TN = START; TN <= TC; TN++) {
    eprintf("Case #%d:\n", TN);
    printf("Case #%d: ", TN);
    try {
      solve();
    } catch (...) {
      eprintf("Caught exception at test case #%d\n", TN);
      return 1;
    }
    fflush(stdout);
    fflush(stderr);
  }
  return 0;
}
