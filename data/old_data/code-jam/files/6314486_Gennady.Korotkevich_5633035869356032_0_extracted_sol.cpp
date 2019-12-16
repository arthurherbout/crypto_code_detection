#include <bits/stdc++.h>

using namespace std;

const int N = 123;

vector < pair <int, int> > pres[123456];

bool solve(vector < vector < pair <int, int> > > a) {
  int n = a.size();
  while (true) {
    bool ch = false;
    for (int i = 0; i < n; i++) {
      if (a[i].empty()) {
        continue;
      }
      for (int j = 0; j < n; j++) {
        if (i == j || a[j].empty()) {
          continue;
        }
        if (a[i].back().second == a[j].back().second && a[i].back().first < a[j].back().first) {
          a[i].pop_back();
          ch = true;
          break;
        }
      }
      if (ch) {
        break;
      }
    }
    if (!ch) {
      break;
    }
  }
  bool any = false;
  for (int i = 0; i < n; i++) {
    if ((int) a[i].size() > 1) {
      any = true;
      break;
    }
  }
  if (!any) {
    return true;
  }
  int empty = -1;
  for (int i = 0; i < n; i++) {
    if (a[i].empty()) {
      empty = i;
      break;
    }
  }
  if (empty == -1) {
    return false;
  }
  for (int i = 0; i < n; i++) {
    if ((int) a[i].size() >= 2) {
      a[empty].push_back(a[i].back());
      a[i].pop_back();
      if (solve(a)) {
        return true;
      }
      a[i].push_back(a[empty].back());
      a[empty].pop_back();
    }
  }
  return false;
}

int main() {
  freopen("in", "r", stdin);
  freopen("out", "w", stdout);
  int pre;
  scanf("%d", &pre);
  for (int i = 0; i < pre; i++) {
    int foo;
    scanf("%d", &foo);
    for (int j = 0; j < foo; j++) {
      int bar, baz;
      scanf("%d %d", &bar, &baz);
      pres[i].push_back(make_pair(bar, baz));
    }
    reverse(pres[i].begin(), pres[i].end());
  }
  int tt;
  scanf("%d", &tt);
  for (int qq = 1; qq <= tt; qq++) {
    printf("Case #%d: ", qq);
    int n, c;
    scanf("%d %d", &n, &c);
    vector < vector < pair <int, int> > > a;
    a.resize(n);
    for (int i = 0; i < n; i++) {
      int id;
      scanf("%d", &id);
      assert((int) pres[id].size() == c);
      a[i] = pres[id];
      for (int j = 0; j < c; j++) {
        cerr << a[i][j].first << " " << a[i][j].second << "    ";
      }
      cerr << endl;
    }
    if (solve(a)) {
      puts("POSSIBLE");
      cerr << "possible" << endl;
    } else {
      puts("IMPOSSIBLE");
      cerr << "impossible" << endl;
    }
    cerr << "test " << qq << "/" << tt << " solved, time = " << clock() << " ms" << endl;
  }
  return 0;
}
