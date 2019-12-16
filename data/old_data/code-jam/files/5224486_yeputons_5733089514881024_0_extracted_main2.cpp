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

const int inf = int(1e9);
const int MAXN = int(1e4) + 10;

int n;
char s[MAXN];

int dyn[MAXN][MAXN];

void solve() {
  scanf("%s", s);
  n = strlen(s);
  forn (i, n) s[i] -= '0';

  vi ne(n + 1, n);
  {
    vi las(10, n);
    for (int i = n - 1; i >= 0; i--) {
      ne[i] = las[s[i]];
      las[s[i]] = i;
    }
  }

  for (int l = n - 1; l >= 0; l--) {
    dyn[l][l] = s[l];

    vi ptrs(10, l);
    for (int v = 1; v <= 9; v++) {
      while (ptrs[v] < n && s[ptrs[v]] != v)
        ptrs[v]++;
    }

    for (int r = l + 1; r < n; r++) {
      int &ans = dyn[l][r] = inf;
      ans = min(ans, s[l] + dyn[l + 1][r]);
      ans = min(ans, dyn[l][r - 1] + s[r]);

      for (int v = 1; v <= 9; v++) if (ptrs[v] < n) {
        for (;;) {
          int x = ne[ptrs[v]];
          if (x <= r) {
            int v1 = l < x ? dyn[l][x - 1] : 0;
            int v2 = x < r ? dyn[x + 1][r] : 0;
            if (v1 < v2) ptrs[v] = x;
            else break;
          } else {
            break;
          }
        }
        int mid = ptrs[v];
        for (int d = 0; d < 2; d++, mid = ne[mid]) {
          if (mid < l || r < mid) continue;

          int cans = 0;
          if (l < mid) cans = max(cans, dyn[l][mid - 1]);
          if (mid < r) cans = max(cans, dyn[mid + 1][r]);
          ans = min(ans, cans + s[mid]);
        }
      }
    }
  }
  printf("%d\n", dyn[0][n - 1]);
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
