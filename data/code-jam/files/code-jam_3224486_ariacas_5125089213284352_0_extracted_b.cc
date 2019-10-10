#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <iostream>
#include <set>
#include <vector>
#include <cstring>
#include <string>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <map>
#include <queue>
using namespace std;
typedef long long ll;
typedef vector<int> vi;
typedef vector<ll> vl;
typedef vector<vi> vvi;
typedef vector<vl> vvl;
typedef vector<double> vd;
typedef pair<int, int> pii;
typedef pair<double, int> pdi;
typedef vector<pii> vii;
typedef vector<string> vs;

bool f(string s, int k) {
  int n = (int) s.length();
  vector<int> pi (n);
  for (int i=1; i<n; ++i) {
    int j = pi[i-1];
    while (j > 0 && s[i] != s[j])
      j = pi[j-1];
    if (s[i] == s[j])  ++j;
    pi[i] = j;
    if (j == k) return 1;
  }
  return 0;
}

vi ts;
int choose(vi & all, const vi & sz) {
/*  ts.clear();
  for (int v : all) {
    for (int i = 0; i < sz[v]; ++i) ts.push_back(v);
  }
  int i = ts[rand() % ts.size()];*/
  ts.resize(all.size());
  ts[0] = sz[all[0]];
  for (int i = 1; i < (int)ts.size(); ++i) ts[i] = ts[i-1] + sz[all[i]];
  int i = upper_bound(ts.begin(), ts.end(), rand() % ts.back()) - ts.begin();
  int res = all[i];
  all.erase(all.begin() + i);
  return res;
}

vvi g;
vi sz;
void dfs(int v) {
  for (int i : g[v]) {
    dfs(i);
    sz[v] += sz[i];
  }
}

int main() {
  int T;
  cin >> T;
  for (int test = 1; test <= T; ++test) {
    int n;
    cin >> n;
    g.assign(n + 1, vi());
    sz.assign(n + 1, 1);
    for (int i = 1; i <= n; ++i) {
      int p; cin >> p;
      g[p].push_back(i);
    }
    dfs(0);
    string s; cin >> s;
    int m; cin >> m;
    vs good(m);
    for (int j = 0; j < m; ++j) {
      cin >> good[j];
    }
    printf("Case #%d: ", test);
    vi cnt(m);
    const int I = 15000;
    //const int I = 150000;
    string con;
    for (int t = 0; t < I; ++t) {
      string res = "";
      vi all;
      for (int i : g[0]) {
        all.push_back(i);
      }
      while (!all.empty()) {
        int i = choose(all, sz);
        res += s[i-1];
        for (int v : g[i]) {
          all.push_back(v);
        }
      }
//      cerr << res << endl;
      for (int j = 0; j < m; ++j) {
        con = good[j] + "#" + res;
        if (f(con, good[j].size())) ++cnt[j];
      }
    }
    for (int j = 0; j < m; ++j) {
      double res = cnt[j] / (double)I;
      res = max(res, 0.029);
      res = min(res, 0.971);
      printf("%.4lf ", res);
    }
    printf("\n");
  }
  return 0;
}
