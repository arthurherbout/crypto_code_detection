#ifdef DEBUG
//#define _GLIBCXX_DEBUG
#endif

#include <bits/stdc++.h>

using namespace std;

mt19937 mrand(random_device{} ()); 

int rnd(int x) {
  return mrand() % x;
}

typedef long double ld;
typedef long long ll;

#ifdef DEBUG
#define eprintf(...) fprintf(stderr, __VA_ARGS__), fflush(stderr)
#else
#define eprintf(...) ;
#endif

#define pb push_back
#define mp make_pair
#define sz(x) ((int) (x).size())
#define TASK "text"

const int inf = (int) 1.01e9;
const ld eps = 1e-9;
const ld pi = acos((ld) -1.0);

struct point {
  long long x, y, z;

  int read() {
    if (scanf("%lld%lld%lld", &x, &y, &z) < 3) {
      return 0;
    }
    return 1;
  }

  point(long long x_ = 0, long long y_ = 0, long long z_ = 0) : x(x_), y(y_), z(z_) {
    assert(max(max(abs(x), abs(y)), abs(z)) <= (long long) 4e18);
  }

  point operator - (const point &p) const {
    return point(x - p.x, y - p.y, z - p.z);
  }

  __int128 operator * (const point &p) const {
    return (__int128) x * p.x + (__int128) y * p.y + (__int128) z * p.z;
  }

  point operator ^ (const point &p) const {
    return point(y * p.z - z * p.y,
                 z * p.x - x * p.z,
                 x * p.y - y * p.x);
  }


  bool operator < (const point &p) const {
    if (x != p.x) {
      return x < p.x;
    }
    if (y != p.y) {
      return y < p.y;
    }
    if (z != p.z) {
      return z < p.z;
    }
    return 0;
  }

  bool operator == (const point &p) const {
    return x == p.x && y == p.y && z == p.z;
  }
};

const int maxn = 5010;
point ps[maxn];

int n;

long long gcd(long long a, long long b) {
  return b ? gcd(b, a % b) : a;
}

int read() {
  if (scanf("%d", &n) < 1) {
    return 0;
  }
  for (int i = 0; i < n; ++i) {
    assert(ps[i].read());
    auto d = gcd(gcd(ps[i].x, ps[i].y), ps[i].z);
    d = abs(d);
    ps[i].x /= d;
    ps[i].y /= d;
    ps[i].z /= d;
  }
  sort(ps, ps + n);
  n = unique(ps, ps + n) - ps;

  shuffle(ps, ps + n, mrand);
  return 1;
}

inline int sgn(__int128 x) {
  return x < 0 ? -1 : (x > 0);
}

point qs[maxn];


#define norm mnorm
point norm;

bool mcomp(const point &a, const point &b) {
  return sgn((norm ^ a) * b) > 0;
}

void solve() {
  for (int i1 = 0; i1 < n; ++i1) {
    for (int j1 = i1 + 1; j1 < n; ++j1) {
      point p1 = ps[i1];
      point p2 = ps[j1];

      auto norm = (p1 ^ p2);
      if (norm == point(0, 0, 0)) {
        printf("YES\n");
        return;
      }
    }
  }

  set<point> used;
  for (int i1 = 0; i1 < n; ++i1) {
    for (int j1 = i1 + 1; j1 < n; ++j1) {
      point p1 = ps[i1];
      point p2 = ps[j1];

      norm = (p1 ^ p2);
      {
    auto d = gcd(gcd(norm.x, norm.y), norm.z);
    d = abs(d);
    norm.x /= d;
    norm.y /= d;
    norm.z /= d;
      }
      if (used.count(norm)) {
        continue;
      }
      used.insert(norm);

      bool fail = 0;

      int cnt = 0;

      int csgn = 0;
      for (int i = 0; i < n; ++i) {
        int val = sgn(norm * ps[i]);
        if (!val) {
          qs[cnt++] = ps[i];
          continue;
        }
        if (csgn && val != csgn) {
          fail = 1;
          break;
        }
        csgn = val;
      }

      if (fail) {
        continue;
      }

      assert(cnt >= 2);

      swap(qs[0], *min_element(qs, qs + cnt, mcomp));
      swap(qs[cnt - 1], *max_element(qs, qs + cnt, mcomp));
      for (int i = 0; i < cnt; ++i) {
        if (mcomp(qs[i], qs[0]) || mcomp(qs[cnt - 1], qs[i])) {
          printf("YES\n");
          return;
        }
      }
      printf("NO\n");
      return;
    }
  }

  printf("YES\n");
}

int main() {
  assert(freopen(TASK ".out", "w", stdout));
  assert(freopen(TASK ".in", "r", stdin));

  int t;
  assert(scanf("%d", &t) == 1);
  t = 0;
  while (read()) {
    printf("Case #%d: ", ++t);
    solve();
    eprintf("Time %.2f\n", (double) clock() / CLOCKS_PER_SEC);
  }
  return 0;
}
