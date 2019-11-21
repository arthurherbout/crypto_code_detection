#include <bits/stdc++.h>

using namespace std;

const double eps = 1e-6;

const int K = 10000;

const int max_n = 22;

int a[42][42];
double b[42][42];

map < int, vector <string> > mp;

long long calc(int n) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      b[i][j] = 0;
    }
  }
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < i; j++) {
      if (a[i][j]) {
        b[i][j]--;
        b[j][i]--;
        b[i][i]++;
        b[j][j]++;
      }
    }
  }
  n--;
  for (int e = 0; e < n; e++) {
    assert(fabs(b[e][e]) > eps);
    for (int i = e + 1; i < n; i++) {
      double coeff = -b[i][e] / b[e][e];
      for (int j = e; j < n; j++) {
        b[i][j] += coeff * b[e][j];
      }
    }
  }
  double ans = 1.0;
  for (int i = 0; i < n; i++) {
    ans *= b[i][i];
  }
  long long res = (long long) (ans + 0.5);
  assert(fabs(ans - res) < eps);
  return res;
}

set < pair <int, int> > mpn;

void dfs(int v) {
  if ((int) mp.size() == K - 1) {
    return;
  }
  if (v == max_n) {
    return;
  }
  for (int t = 1; t < (1 << v); t++) {
    if ((int) mp.size() == K - 1) {
      return;
    }
    for (int i = 0; i < v; i++) {
      a[v][i] = (t & (1 << (v - i - 1))) ? 1 : 0;
    }
    long long cnt = calc(v + 1);
    if (cnt > K) {
      continue;
    }
    if (mp.find(cnt) == mp.end()) {
      vector <string> z(v + 1, string(v + 1, '0'));
      for (int i = 0; i < v + 1; i++) {
        for (int j = 0; j < v + 1; j++) {
          if (a[i][j]) {
            z[i][j] = z[j][i] = '1';
          }
        }
      }
      mp[cnt] = z;
      cerr << "found cnt = " << cnt << " with n = " << v + 1 << "; time = " << clock() << " ms; mp.size() = " << mp.size() << endl;
    }
    if (mpn.find(make_pair(cnt, v + 1)) == mpn.end()) {
      mpn.insert(make_pair(cnt, v + 1));
      dfs(v + 1);
    }
  }
}

int main() {
/*  int n = 10;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < i; j++) {
      a[i][j] = 0;
    }
  }
  for (int i = 1; i < n; i++) {
    a[i][i - 1] = 1;
  }
  a[n - 1][0] = 1;
  cerr << "calc = " << calc(n) << endl;*/
  dfs(1);
  int tt;
  scanf("%d", &tt);
  for (int qq = 1; qq <= tt; qq++) {
    printf("Case #%d: ", qq);
    int k;
    scanf("%d", &k);
    int n = mp[k].size();
    printf("%d\n", n);
    for (int i = 0; i < n; i++) {
      printf("%s\n", mp[k][i].c_str());
    }
  }
  return 0;
}
