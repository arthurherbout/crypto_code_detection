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

void pre() {
}

int test = 1;
void _out(ll x) {
  cout << "Case #" << test++ << ": " << x << endl;
}
#define out(...) {_out(__VA_ARGS__); return; }

const int B = 1e6 + 5;
int n;
vector<int> g[B];
int mt[B];
int rmt[B];

int used[B];
int VER;

mt19937 rr;

void scan() {
  ++VER;
  forn (i, B) {
    g[i].clear();
    rmt[i] = -1;
    mt[i] = -1;
  }

  cin >> n;
  forn (i, n) {
    forn (j, 6) {
      int u;
      cin >> u;
      assert(u >= 0 && u < B);
      g[u].push_back(i);
    }
  }
  forn (i, B) {
    shuffle(all(g[i]), rr);
  }
}

bool kuhn(int u) {
  used[u] = VER;
  for (int v : g[u]) {
    if (mt[v] == -1 || (used[mt[v]] != VER && kuhn(mt[v]))) {
      rmt[u] = v;
      mt[v] = u;
      return true;
    }
  }
  return false;
}

void solve() {
  int res = 0;
  int r = 0;
  for (int l = 0; l < B; ++l) {
    r = max(r, l);
    while (r < B) {
      ++VER;
      if (kuhn(r)) {
        ++r;
      } else {
        break;
      }
    }

    res = max(res, r - l);
    if (rmt[l] != -1) {
      int v = rmt[l];
      mt[v] = -1;
      rmt[l] = -1;
    }
  }
  out(res);
}

int main() {
#ifdef LOCAL
  assert(freopen("a.in", "r", stdin));
#endif
  pre();
  int tn;
  cin >> tn;
  forn (i, tn) {
    scan();
    solve();
  }
}
