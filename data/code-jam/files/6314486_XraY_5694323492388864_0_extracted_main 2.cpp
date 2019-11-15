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

const int maxn = 200;
int n;

int xs[maxn][3];

int read() {
  if (scanf("%d", &n) < 1) {
    return 0;
  }
  n += 2;
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < 3; ++j) {
      scanf("%d", xs[i] + j);
    }
  }
  return 1;
}

#define left mleft
set<int> left[maxn];

const int K = (int) 6e3 + 100;
int dist[maxn][K];

void solve() {
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < K; ++j) {
      left[i].insert(j);
      dist[i][j] = inf;
    }
  }
  left[0].erase(0);

  dist[0][0] = 0;
  vector<pair<int, int> > st;
  st.pb(mp(0, 0));

  for (int l = 0; l < sz(st); ++l) {
    auto top = st[l];
    int v = top.first;
    int di = top.second;

    int val = dist[v][di];

    if (!di || v >= 2) {
      for (int u = 0; u < n; ++u) {
        int diff = 0;
        for (int j = 0; j < 3; ++j) {
          diff += abs(xs[v][j] - xs[u][j]);
        }
        int mn = abs(diff - di);
        int mx = min(K - 1, diff + di);
        while (1) {
          auto iter = left[u].lower_bound(mn);
          if (iter != left[u].end() && *iter <= mx) {
            //eprintf("got (%d,%d)\n", u, *iter);
            assert(dist[u][*iter] == inf);
            dist[u][*iter] = val + 1;
            if (u == 1 && *iter == 0) {
              printf("%d\n", dist[1][0] - 1);
              return;
            }
            st.pb(mp(u, *iter));
            left[u].erase(iter);
            continue;
          }
          break;
        }
      }
    }
  }

  if (dist[1][0] == inf) {
    printf("IMPOSSIBLE\n");
    return;
  }
  printf("%d\n", dist[1][0] - 1);
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
