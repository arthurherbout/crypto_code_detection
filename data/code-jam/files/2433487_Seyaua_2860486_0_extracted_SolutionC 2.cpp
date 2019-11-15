#include <iostream>
#include <string>
#include <cstdio>
#include <cstring>
#include <set>
#include <map>
#include <queue>
#include <algorithm>
#include <vector>
#include <bitset>
#include <cmath>
#pragma comment(linker, "/STACK:256000000")

using namespace std;

const int maxN = 2100;

int n, m, p;
int u[maxN];
int v[maxN];
int a[maxN];
int b[maxN];
int P[maxN];

vector <pair<long long, int> > g1[maxN], g2[maxN];
vector<int> id[maxN];

const long long INF = 1000000000000000000LL;
long long d[maxN];
long long par[maxN];

bool used[maxN];

bool check(int key) {
  {
    memset(used, 0, sizeof(used));
    memset(par, 0, sizeof(par));
    for (int i = 0; i < key; ++i) {
      used[P[i]] = true;
    }

    int current = 1;
    if (key > 0) {
      current = v[P[key - 1]];
    }

    for (int i = 1; i <= n; ++i) {
      d[i] = INF;
    }
    d[current] = 0;

    set<pair<long long, int> > S;
    S.insert(make_pair(d[current], current));
    while (!S.empty()) {
      int v = S.begin()->second;
      S.erase(S.begin());

      for (int i = 0; i < g1[v].size(); ++i) {
        long long w = g1[v][i].second;
        int u = g1[v][i].first;

        if (used[id[v][i]]) {
          continue;
        }

        if (d[u] > d[v] + w) {
          S.erase(make_pair(d[u], u));
          d[u] = d[v] + w;
          par[u] = id[v][i];
          S.insert(make_pair(d[u], u));
        }
      }
    }

    current = 2;
    if (d[current] != INF) {
      while (par[current]) {
        used[par[current]] = true;
        current = u[par[current]];
      }

      long long have = d[2];
      for (int i = 0; i < key; ++i) {
        have += a[P[i]];
      }

      for (int i = 1; i <= n; ++i) {
        d[i] = INF;
      }
      d[1] = 0;

      set<pair<long long, int> > S;
      S.clear();
      S.insert(make_pair(d[current], current));
      while (!S.empty()) {
        int v = S.begin()->second;
        S.erase(S.begin());

        for (int i = 0; i < g1[v].size(); ++i) {
          long long w = g2[v][i].second;
          if (used[id[v][i]]) {
            w = g1[v][i].second;
          }

          int u = g1[v][i].first;
          if (d[u] > d[v] + w) {
            S.erase(make_pair(d[u], u));
            d[u] = d[v] + w;
            par[u] = id[v][i];
            S.insert(make_pair(d[u], u));
          }
        }
      }

      if (d[2] == have) {
        return true;
      }
    }
  }


  {
    memset(used, 0, sizeof(used));
    memset(par, 0, sizeof(par));
    for (int i = 0; i < key; ++i) {
      used[P[i]] = true;
    }

    int current = 1;
    if (key > 0) {
      current = v[P[key - 1]];
    }

    for (int i = 1; i <= n; ++i) {
      d[i] = INF;
    }
    d[current] = 0;

    set<pair<long long, int> > S;
    S.insert(make_pair(d[current], current));
    while (!S.empty()) {
      int v = S.begin()->second;
      S.erase(S.begin());

      for (int i = 0; i < g1[v].size(); ++i) {
        long long w = g2[v][i].second;
        int u = g1[v][i].first;

        if (used[id[v][i]]) {
          continue;
        }

        if (d[u] > d[v] + w) {
          S.erase(make_pair(d[u], u));
          d[u] = d[v] + w;
          par[u] = id[v][i];
          S.insert(make_pair(d[u], u));
        }
      }
    }

    current = 2;
    if (d[current] != INF) {
      long long have = d[2];
      for (int i = 0; i < key; ++i) {
        have += a[P[i]];
      }

      for (int i = 1; i <= n; ++i) {
        d[i] = INF;
      }
      d[1] = 0;

      set<pair<long long, int> > S;
      S.clear();
      S.insert(make_pair(d[current], current));
      while (!S.empty()) {
        int v = S.begin()->second;
        S.erase(S.begin());

        for (int i = 0; i < g1[v].size(); ++i) {
          long long w = g2[v][i].second;
          if (used[id[v][i]]) {
            w = g1[v][i].second;
          }

          int u = g1[v][i].first;
          if (d[u] > d[v] + w) {
            S.erase(make_pair(d[u], u));
            d[u] = d[v] + w;
            par[u] = id[v][i];
            S.insert(make_pair(d[u], u));
          }
        }
      }

      if (d[2] == have) {
        return true;
      }
    }
  }

  return false;
}

void solve(int tt) {
  printf("Case #%d: ", tt + 1);
  scanf("%d%d%d", &n, &m, &p);

  for (int i = 1; i <= n; ++i) {
    g1[i].clear();
    g2[i].clear();
    id[i].clear();
  }

  for (int i = 0; i < m; ++i) {
    scanf("%d%d%d%d", &u[i], &v[i], &a[i], &b[i]);
    g1[u[i]].push_back(make_pair(v[i], a[i]));
    g2[u[i]].push_back(make_pair(v[i], b[i]));
    id[u[i]].push_back(i);
  }

  for (int i = 0; i < p; ++i) {
    scanf("%d", &P[i]);
    --P[i];
  }

  int l = 0, r = p;
  int res = 0;
  while (l <= r) {
    int key = (l + r) / 2;
    if (check(key)) {
      res = key;
      l = key + 1;
    } else {
      r = key - 1;
    }
  }
  if (res != p) {
    cout << P[res] + 1 << endl;
  } else {
    cout << "Looks Good To Me" << endl;
  }
}

int main() {
  freopen("input.txt", "r", stdin);
  freopen("output.txt", "w", stdout);

  int t;
  cin >> t;
  for (int i = 0; i < t; ++i) {
    solve(i);
    cerr << i << endl;
  }

  return 0;
}
