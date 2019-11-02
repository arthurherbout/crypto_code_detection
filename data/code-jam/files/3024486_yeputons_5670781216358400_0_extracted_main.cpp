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

typedef vector<ll> vll;
typedef vector<int> vi;
typedef vector<vi> vvi;
typedef vector<bool> vb;
typedef vector<vb> vvb;
typedef pair<int, int> pii;

const int inf = 1e9;
int n;
vvi es;
vvi ds;

bool containsE(int s, int e, int a, int b) {
  int nd = ds[s][e];
  assert(nd < inf);
  if (nd == ds[s][a] + 1 + ds[b][e]) return true;
  if (nd == ds[s][b] + 1 + ds[a][e]) return true;
  return false;
}
bool containsV(int s, int e, int v) {
  if (v == e) return false;
  return ds[s][e] == ds[s][v] + ds[v][e];
}

/*struct st {
  int s1; int s2; int v1; int v2; bool ate1; bool ate2; int emptyTurns;
  bool operator<(const st &b) const {
    #define cmp(x) if (x != b.x) return x < b.x;
    cmp(s1)
    cmp(s2)
    cmp(v1)
    cmp(v2)
    cmp(ate1)
    cmp(ate2)
    cmp(emptyTurns)
    return false;
    #undef cmp
  }
};*/

int cache[80][80][80][80][2][2][2];
vi coins;
int solve(int s1, int s2, int v1, int v2, bool ate1, bool ate2, int emptyTurns = 0) {
  if (emptyTurns >= 2) return 0;
  if (s1 < 0) {
    assert(v1 < 0);
    int ans = -inf;
    forn (i, n)
      ans = max(ans, -solve(s2, i, v2, i, ate2, ate1));
    return ans;
  }
  assert(s1 >= 0 && s2 >= 0);
  assert(v1 >= 0 && v2 >= 0);

  int &ans = cache[s1][s2][v1][v2][ate1][ate2][emptyTurns] = -inf;
  if (ans >= 0) return ans;

  bool hasTurn = false;
  for (int b1 : es[v1]) {
    if (containsE(s1, v1, v1, b1)) continue;
    if (containsE(s2, v2, v1, b1)) continue;

    int cans = -solve(s2, s1, v2, b1, ate2, ate1);
    hasTurn = true;
    ans = max(ans, cans);
  }
  if (!hasTurn) {
    assert(ans == -inf);
    ans = -solve(s2, s1, v2, v1, ate2, true, emptyTurns + 1);
  } else {
    assert(ans > -inf);
  }
  if (!containsV(s2, v2, v1)) {
    if (!ate1 && (!ate2 || v2 != v1))
      ans += coins[v1];
  }
  return ans;
}

void solve() {
  scanf("%d", &n);
  coins = vi(n);
  for (int &x : coins) scanf("%d", &x);

  es = vvi(n);
  forn (a, n - 1) {
    int b;
    scanf("%d", &b), b--;
    assert(0 <= b && b < n);
    es[a].pb(b);
    es[b].pb(a);
  }

  eprintf("n=%d\n", n);
  ds = vvi(n, vi(n, inf));
  forn (i, n) {
    ds[i][i] = 0;
    for (int i2 : es[i])
      ds[i][i2] = 1;
  }
  forn (i, n)
  forn (j, n)
  forn (k, n)
    ds[j][k] = min(ds[j][k], ds[j][i] + ds[i][k]);

  memset(cache, -1, sizeof cache);
  int ans = solve(-1, -1, -1, -1, false, false);
  printf("%d\n", ans);
}

bool endsWith(string a, string b) {
  return a.length() >= b.length() && string(a, a.length() - b.length()) == b;
}

int main(int argc, char *argv[]) {
  {
    string fn = "";
    if (argc >= 2) fn = argv[1];
    if (endsWith(fn, ".in")) fn = string(fn, 0, fn.length() - 3);
    freopen((fn + ".in").c_str(), "r", stdin);
    freopen((fn + ".out").c_str(), "w", stdout);
  }

  int TC;
  assert(scanf("%d", &TC) >= 1);
  for (int TN = 1; TN <= TC; TN++) {
    eprintf("Case #%d:\n", TN);
    printf("Case #%d: ", TN);
    try {
      solve();
    } catch (const exception &e) {
      eprintf("Catched exception at test case #%d: %s\n", TN, e.what());
      return 1;
    } catch (...) {
      eprintf("Catched exception at test case #%d\n", TN);
      return 1;
    }
  }
  return 0;
}
