#include <bits/stdc++.h>
using namespace std;
#define sz(x) ((int) (x).size())
#define forn(i,n) for (int i = 0; i < int(n); ++i)
#define all(x) (x).begin(), (x).end()
typedef long long ll;
typedef long long i64;
typedef long double ld;
typedef pair<int, int> pii;
const int inf = int(1e9) + int(1e5);
const ll infl = ll(2e18) + ll(1e10);

void pre() {
}

int test = 1;
void _out(ll x) {
  if (x == -1)
    cout << "Case #" << test++ << ": " << "IMPOSSIBLE" << endl;
  else
    cout << "Case #" << test++ << ": " << x << endl;
}
#define out(...) {_out(__VA_ARGS__); return; }

struct pt {
  int x, y, z;

  pt operator-(const pt &p) const {
    return pt{x - p.x, y - p.y, z - p.z};
  }

  int abs() const {
    return ::abs(x) + ::abs(y) + ::abs(z);
  }

  void read() {
    cin >> x >> y >> z;
  }
};

int n;
vector<pt> p;
pt S, T;

void scan() {
  cin >> n;
  p.resize(n);
  S.read();
  T.read();
  forn (i, n) {
    p[i].read();
  }
}

const int maxn = 205;
set<int> unused[maxn];
const int B = 1000 * 6 + 5;

vector<pii> q;
int dist[maxn][B];

void solve() {
  q.clear();
  forn (i, n) {
    unused[i].clear();
    forn (j, B) {
      unused[i].insert(j);
      dist[i][j] = inf;
    }
    unused[i].insert(inf);
  }
  forn (i, n) {
    unused[i].erase((p[i] - S).abs());
    dist[i][(p[i] - S).abs()] = 1;
    q.emplace_back(i, (p[i] - S).abs());
  }
  forn (ii, sz(q)) {
    int u, d;
    tie(u, d) = q[ii];
    if (d == (p[u] - T).abs()) {
      out(dist[u][d]);
    }
    //cerr << "from " << u << ' ' << d << endl;
    forn (to, n) {
      if (to == u)
        continue;
      int dst = (p[u] - p[to]).abs();
      int mn = max(0, abs(dst - d));
      int mx = min(B - 1, dst + d);
      while (true) {
        int x = *unused[to].lower_bound(mn);
        if (x > mx)
          break;
        //cerr << "to " << to << ' ' << x << '\n';
        dist[to][x] = dist[u][d] + 1;
        unused[to].erase(x);
        q.emplace_back(to, x);
      }
    }
  }
  out(-1);
}

int main() {
#ifdef LOCAL
  assert(freopen("f.in", "r", stdin));
#endif
  pre();
  int tn;
  cin >> tn;
  forn (i, tn) {
    scan();
    solve();
  }
}

