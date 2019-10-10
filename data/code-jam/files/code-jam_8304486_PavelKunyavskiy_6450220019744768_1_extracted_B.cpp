#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>

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

const int MAXN = 1000;

vector<int> g[MAXN];
vector<int> a;
vector<int> b;
vector<int> used;
vector<int> ans;
vector<int> bal;

void add_flow(int id, int from, int val) {
  if (from != a[id]) {
    assert(from == b[id]);
    val = -val;
  }
  ans[id] = val;
  bal[a[id]] -= val;
  bal[b[id]] += val;
}

void dfs(int v, int pid = -1) {
  used[v] = 1;
  for (int id : g[v]) {
    if (id == pid) continue;
    int to = a[id] + b[id] - v;
    if (used[to] == 2) {
      continue;
    }
    if (used[to] == 1) {
      add_flow(id, v, 1);
      continue;
    }
    dfs(to, id);
  }
  if (pid != -1) {
    add_flow(pid, v, bal[v]);
  }
  assert(bal[v] == 0);
  used[v] = 2;
}

void solve() {
  int f, p;
  scanf("%d%d", &f, &p);
  a.resize(p);
  b.resize(p);
  used = vector<int>(f, 0);
  ans = vector<int>(p, 0);
  bal = vector<int>(f, 0);
  for (int i = 0; i < f; i++) {
    g[i].clear();
  }
  for (int i = 0; i < p; i++) {
    scanf("%d%d",&a[i], &b[i]);
    --a[i], --b[i];
    g[a[i]].push_back(i);
    g[b[i]].push_back(i);
  }
  for (int i = 0; i < f; i++) {
    if (used[i] == 0) {
      dfs(i);
    }
  }
  for (int i = 0; i < p; i++) {
    if (ans[i] == 0) {
      printf("IMPOSSIBLE\n");
      return;
    }
  }
  for (int i = 0; i < p; i++) {
    printf("%d ", ans[i]);
    assert(abs(ans[i]) <= f * f);
  }
  printf("\n");
}


int main() {
//  freopen("b.in", "r", stdin);
//  freopen("b.out", "w", stdout);

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