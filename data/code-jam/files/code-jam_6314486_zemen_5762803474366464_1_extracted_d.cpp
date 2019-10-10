#include <bits/stdc++.h>
using namespace std;
#define sz(x) ((int) (x).size())
#define forn(i,n) for (int i = 0; i < int(n); ++i)
#define all(x) (x).begin(), (x).end()
typedef long long ll;
typedef long long i64;
typedef long double ld;
const int inf = int(1e9) + int(1e5);
const ll infl = ll(2e18) + ll(1e10);

const ld eps = 1e-9;
bool ze(ld x) {
  return fabsl(x) < eps;
}
struct pt {
  ld x, y, z;

  pt operator+(const pt &p) const {
    return pt{x + p.x, y + p.y, z + p.z};
  }

  pt operator-(const pt &p) const {
    return pt{x - p.x, y - p.y, z - p.z};
  }

  ld operator*(const pt &p) const {
    return x * p.x + y * p.y + z * p.z;
  }

  pt operator*(ld a) const {
    return pt{x * a, y * a, z * a};
  }

  pt operator%(const pt &p) const {
    return pt{y * p.z - z * p.y,
      z * p.x - x * p.z,
      x * p.y - y * p.x};
  }

  ld abs() const {
    return sqrtl(*this * *this);
  }

  ld abs2() const {
    return *this * *this;
  }

  pt norm() const {
    ld d = abs();
    return pt{x / d, y / d, z / d};
  }
};

ld v(pt a, pt b, pt c) {
  return (a % b) * c;
}

void pre() {
}

int test = 1;
void _out(string s) {
  cout << "Case #" << test++ << ": " << s << endl;
}
#define out(...) {_out(__VA_ARGS__); return; }

int n;
vector<pt> p;

void scan() {
  cin >> n;
  p.resize(n);
  forn (i, n) {
    cin >> p[i].x >> p[i].y >> p[i].z;
  }
}

void solve() {
  vector<pt> np;
  forn (i, n) {
    bool uni = true;
    for (pt v : np) {
      if (ze((v % p[i]).abs2())) {
        if (p[i] * v < 0) {
          out("YES");
        } else {
          uni = false;
        }
      }
    }
    if (uni) {
      np.push_back(p[i]);
    }
  }
  p = np;
  n = sz(np);
  //for (auto pp : p) {
    //cerr << pp.x << ' ' << pp.y << ' ' << pp.z << endl;
  //}

  forn (i, n) {
    int k = i ? 0 : 1;
    forn (j, n) {
      if (j == i || j == k)
        continue;
      ld V = v(p[i], p[k], p[j]);
      if (!ze(V) && V > 0) {
        k = j;
      }
    }
    bool ok = true;
    forn (j, n) {
      if (j == i || j == k)
        continue;
      ld V = v(p[i], p[k], p[j]);
      if (ze(V)) {
        pt norm = p[i] % p[k];
        pt norm2 = p[i] % p[j];
        ld S = norm * norm2;
        if (S < 0) {
          ok = false;
        }
      } else if (V > 0) {
        ok = false;
      }
    }
    if (ok) {
      //cerr << i << ' ' << k << endl;
      out("NO");
    }
  }
  out("YES");
  /*
  forn (i, n) {
    forn (j, n) {
      if (i == j)
        continue;
      bool pos = false, neg = false;
      bool fail = false;
      pt norm = p[i] % p[j];
      forn (k, n) {
        if (k == i || k == j)
          continue;
        ld cv = v(p[i], p[j], p[k]);
        if (ze(cv)) {
          pt norm2 = p[i] % p[k];
          if (norm2 * norm < 0) {
            fail = true;
          }
        } else if (cv > 0) {
          pos = true;
        } else if (cv < 0) {
          neg = true;
        }
      }
      if (pos && neg) {
        fail = true;
      }
      if (!fail) {
        //cerr << "fail " << i << ' ' << j << endl;
        out("NO");
      }
    }
  }
  out("YES");
  */
}

int main() {
#ifdef LOCAL
  assert(freopen("d.in", "r", stdin));
#endif
  pre();
  int tn;
  cin >> tn;
  forn (i, tn) {
    scan();
    solve();
  }
}

