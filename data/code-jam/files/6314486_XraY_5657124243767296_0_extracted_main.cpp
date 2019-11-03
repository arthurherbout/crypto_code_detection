#ifdef DEBUG
#define _GLIBCXX_DEBUG
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


const int maxc = (int) 1e6 + 10;
vector<int> es[maxc];

int n;

int pr[maxc];
int back[maxc];

int read() {
  if (scanf("%d", &n) < 1) {
    return 0;
  }
  for (int i = 0; i < maxc; ++i) {
    es[i].clear();
    back[i] = -1;
  }

  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < 6; ++j) {
      int x;
      scanf("%d", &x);
      es[x].pb(i);
    }
  }
  return 1;
}

int link[maxc];

int used[maxc];

int mn;

int maxu = 0;

bool dfs(int v) {
  used[v] = maxu;
  mn = min(mn, v);
  for (int u : es[v]) {
    if (pr[u] != -1 && used[pr[u]] < maxu) {
      link[u] = v;
    }
    if (pr[u] == -1 || (used[pr[u]] < maxu && dfs(pr[u]))) {
      pr[u] = v;
      back[v] = u;
      return 1;
    }
  }
  return 0;
}

void solve() {
  for (int i = 0; i < n; ++i) {
    pr[i] = -1;
  }

  int res = 0;

  int l = 0;
  for (int r = 0; r < maxc; ++r) {
    ++maxu;
    if (maxc - l <= res) {
      break;
    }

    mn = r;
    if (!dfs(r)) {
      for (int x = mn, y = back[x]; x != r;) {
        int nx = link[y];
        int ny = back[nx];

        pr[y] = nx;
        back[nx] = y;

        x = nx, y = ny;
      }
      back[mn] = -1;
      while (l < mn) {
        assert(back[l] != -1);
        pr[back[l]] = -1;
        back[l] = -1;
        ++l;
      }
      ++l;
    }
    res = max(res, r - l + 1);
  }
  printf("%d\n", res);
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
