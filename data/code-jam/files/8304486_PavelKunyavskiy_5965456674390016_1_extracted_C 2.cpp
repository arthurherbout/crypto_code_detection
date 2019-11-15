#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <algorithm>

#ifdef LOCAL
#define eprintf(...) fprintf(stderr,__VA_ARGS__)
#else
#define eprintf(...)
#endif

#define TIMESTAMP(x) eprintf("["#x"] Time : %.3lf s.\n", clock()*1.0/CLOCKS_PER_SEC)
#define TIMESTAMPf(x, ...) eprintf("[" x "] Time : %.3lf s.\n", __VA_ARGS__, clock()*1.0/CLOCKS_PER_SEC)

using namespace std;

typedef long long ll;
typedef long double ld;

void PreCalc() {
}

int n;
vector<int> E;
vector<int> D;
vector<int> L;
vector<int> go;

int get_wait_time(int d1, int l1, int d2) {
  d1 = (d1 + l1) % 24;
  return (d2 - d1 + 24) % 24;
}

int get_cost(int pos) {
  if (pos == 0) {
    return min(D[0] + get_wait_time(D[go[1]], L[go[1]], D[1]),
               D[1] + get_wait_time(D[go[0]], L[go[0]], D[0]));
  }
  return get_wait_time(D[go[2 * pos]], L[go[2 * pos]], D[2 * pos]) +
         get_wait_time(D[go[2 * pos + 1]], L[go[2 * pos + 1]], D[2 * pos + 1]);
}


struct edge {
  int a, b, w;
};

vector<int> p;

int get(int a) {
  if (a != p[a]) return p[a] = get(p[a]);
  return a;
}

int join(int a, int b) {
  a = get(a);
  b = get(b);
  if (a == b) return 0;
  p[a] = b;
  return 1;
}

int solve_slow() {
  int ans = -1;

  for (int i = 0; i < (1 << n); i++) {
    vector<int> ptr(n);

    int cur = 0;
    int cnt = 0;
    int v = 0;
    do {
      int pos;
      if (ptr[v] == 2) {
        break;
      }
      if (ptr[v] ^ ((i >> v) & 1)) {
        pos = 2 * v;
      } else {
        pos = 2 * v + 1;
      }
      ptr[v]++;

      cur += (D[pos] % 24 - cur % 24 + 24) % 24;
      assert(cur % 24 == D[pos] % 24);
      cur += L[pos];

      cnt += 1;
      v = E[pos];
    } while (1);

    if (cnt == 2 * n) {
      if (ans == -1 || ans > cur) {
        ans = cur;
      }
    }
  }


  fprintf(stderr, "slow = %d\n", ans);
  return ans;
}



void solve() {
  scanf("%d", &n);
  E = vector<int>(2 * n);
  D = vector<int>(2 * n);
  L = vector<int>(2 * n);
  go = vector<int>(2 * n, -1);

  int ans = 0;

  for (int i = 0; i < 2 * n; i++) {
    scanf("%d%d%d", &E[i], &D[i], &L[i]);
    --E[i];
    ans += L[i];
    if (go[2 * E[i]] == -1) {
      go[2 * E[i]] = i;
    } else if (go[2 * E[i] + 1] == -1) {
      go[2 * E[i] + 1] = i;
    } else {
      assert(0);
    }
  }
//  int ans_real = solve_slow();

  for (int i = 0; i < n; i++) {
    int now = get_cost(i);
    swap(go[2 * i], go[2 * i + 1]);
    int other = get_cost(i);

    if (now < other) {
      swap(go[2 * i], go[2 * i + 1]);
    }
  }


  for (int i = 0; i < n; i++) {
    ans += get_cost(i);
  }

  vector<int> comp(2 * n, -1);
  int cc = 0;
  for (int i = 0; i < 2 * n; i++) {
    if (comp[i] != -1) continue;
    for (int j = i; comp[j] == -1; j = go[j]) {
      comp[j] = cc;
    }
    ++cc;
  }

  vector<edge> es;

  for (int i = 0; i < n; i++) {
    int now = get_cost(i);
    swap(go[2 * i], go[2 * i + 1]);
    int other = get_cost(i);
    swap(go[2 * i], go[2 * i + 1]);

    es.push_back({comp[2 * i], comp[2 * i + 1], other - now});
  }

  sort(es.begin(), es.end(), [](const edge &a, const edge &b) { return a.w < b.w; });

  p = vector<int>(cc);
  for (int i = 0; i < cc; i++) {
    p[i] = i;
  }

  for (const edge &e : es) {
    if (join(e.a, e.b)) {
      ans += e.w;
      cc--;
    }
  }

  assert(cc == 1);
  printf("%d\n", ans);
//  assert(ans == solve_slow());
//  assert(ans == ans_real);
//  printf("%d\n", ans_real);
}


int main() {
//  freopen("c.in", "r", stdin);
//  freopen("c.out", "w", stdout);

  PreCalc();
  TIMESTAMP(PreCalc);

  char buf[1000];
  int testNum;
  fgets(buf, sizeof buf, stdin);
  sscanf(buf, "%d", &testNum);

  for (int testId = 1; testId <= testNum; testId++) {
    if (testId <= 20 || testId >= testNum - 20 || testId % 10 == 0)
      TIMESTAMPf("Test %d", testId);
    printf("Case #%d: ", testId);
    solve();
  }

  TIMESTAMP(end);
  return 0;
}