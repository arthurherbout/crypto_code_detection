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

const int STS = 66000;
vector<pii> p[STS];

void pre() {
  int sts;
  cin >> sts;
  forn (i, sts) {
    int x;
    cin >> x;
    p[i].resize(x);
    forn (j, x) {
      cin >> p[i][j].first;
      cin >> p[i][j].second;
    }
    reverse(all(p[i]));
  }
}

int test = 1;
void _out(bool x) {
  if (x)
    cout << "Case #" << test++ << ": " << "POSSIBLE" << endl;
  else
    cout << "Case #" << test++ << ": " << "IMPOSSIBLE" << endl;
}
#define out(...) {_out(__VA_ARGS__); return; }

void reduce(vector<vector<pii>> &v) {
  while (true) {
    bool ch = false;
    forn (i, sz(v))
      forn (j, i) {
        if (!v[i].empty() && !v[j].empty() && v[i].back().second == v[j].back().second) {
          ch = true;
          if (v[i].back().first > v[j].back().first)
            v[j].pop_back();
          else
            v[i].pop_back();
        }
      }
    if (!ch)
      break;
  }
}

int iters = 0;
bool rec(vector<vector<pii>> s) {
  if (iters > int(1e7))
    return false;
  ++iters;
  reduce(s);
  /*
  cerr << "rec\n";
  for (auto v: s) {
    for (auto p: v) {
      cerr << p.first << ' ' << p.second << ' ';
    }
    cerr << endl;
  }
  */
  bool win = true;
  forn (i, sz(s)) {
    if (sz(s[i]) > 1)
      win = false;
  }
  if (win)
    return true;
  sort(all(s), [](const vector<pii> &a, const vector<pii> &b) {
        return sz(a) > sz(b);
      });
  if (!s.back().empty())
    return false;
  for (int i = 0; i < sz(s) && !s[i].empty(); ++i) {
    auto t = s;
    t.back().push_back(t[i].back());
    t[i].pop_back();
    if (rec(t))
      return true;
  }
  return false;
}

void scan() {
  iters = 0;
  int n, c;
  cin >> n >> c;
  vector<vector<pii>> s;
  set<int> sdiff;
  forn (i, n) {
    int id;
    cin >> id;
    s.push_back(p[id]);
    for (auto x: p[id]) {
      sdiff.insert(x.second);
    }
  }
  if (sz(sdiff) > n) {
    out(false);
  }

  if (rec(s)) {
    out(true);
  } else {
    out(false);
  }
}

void solve() {
}

int main() {
#ifdef LOCAL
  assert(freopen("e.in", "r", stdin));
#endif
  pre();
  int tn;
  cin >> tn;
  forn (i, tn) {
    cerr << "test " << i + 1 << endl;
    scan();
    solve();
  }
}

