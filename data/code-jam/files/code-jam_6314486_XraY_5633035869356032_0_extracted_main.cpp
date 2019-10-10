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

const int maxp = (int) 6e4 + 10;
vector<pair<int, int> > cards[maxp];
int p;

int n, c;

vector<vector<pair<int, int> > > cur;

int read() {
  if (scanf("%d%d", &n, &c) < 2) {
    return 0;
  }
  cur.resize(n);
  for (int i = 0; i < n; ++i) {
    int pos;
    scanf("%d", &pos);
    cur[i] = cards[pos];
  }
  return 1;
}

map<vector<vector<pair<int, int> > >, bool> mem;

bool get(vector<vector<pair<int, int> > > cur) {
  sort(cur.begin(), cur.end());
  if (mem.count(cur)) {
    return mem[cur];
  }
  auto &res = mem[cur];
  res = 0;

  {
    bool ok = 1;
    for (int i = 0; i < n; ++i) {
      if (sz(cur[i]) > 1) {
        ok = 0;
        break;
      }
    }
    if (ok) {
      res = 1;
      return res;
    }
  }
  assert(sz(cur) == n);
  for (int i = 0; i < n; ++i) {
    if (!sz(cur[i])) {
      for (int j = 0; j < n; ++j) {
        if (sz(cur[j])) {
          auto was = cur[j].back();
          cur[i].pb(was);
          cur[j].pop_back();

          if (get(cur)) {
            res = 1;
          }
          cur[i].pop_back();
          cur[j].pb(was);

          if (res) {
            return 1;
          }
        }
      }
    } else {
      auto was = cur[i].back();
      for (int j = 0; j < n; ++j) {
        if (j == i) {
          continue;
        }
        if (sz(cur[j]) && cur[i].back().second == cur[j].back().second && cur[i].back().first <= cur[j].back().first) {
          cur[i].pop_back();
          if (get(cur)) {
            res = 1;
          }
          cur[i].pb(was);

          if (res) {
            return 1;
          }
          break;
        }
      }
    }
  }
  return res;
}

void solve() {
  if (get(cur)) {
    printf("POSSIBLE\n");
  } else {
    printf("IMPOSSIBLE\n");
  }
}

int main() {
  assert(freopen(TASK ".out", "w", stdout));
  assert(freopen(TASK ".in", "r", stdin));

  scanf("%d", &p);
  for (int iter = 0; iter < p; ++iter) {
    int k;
    scanf("%d", &k);
    auto &cur = cards[iter];
    cur.resize(k);
    for (int i = 0; i < k; ++i) {
      scanf("%d%d", &cur[i].first, &cur[i].second);
    }
    reverse(cur.begin(), cur.end());
  }

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
