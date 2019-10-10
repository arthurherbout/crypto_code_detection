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

double dp[1 << 20];
int n;

double solve2(int mask) {
  if (mask == (1 << (n)) - 1) {
    return 0.;
  }
  if (dp[mask] > -0.5) {
    return dp[mask];
  }
  double res = 0;

  for (int i = 0; i < n; ++i) {
    int add = 0;
    int cmask = mask;
    for (int j = i;; ++j) {
      int nj = j % n;
      if (mask & (1 << nj)) {
        ++add;
      } else {
        cmask += (1 << nj);
        break;
      }
    }
    res += 1. / n * (n - add + solve2(cmask));
  }
  return dp[mask] = res;
}

void solve(int tt) {
  printf("Case #%d: ", tt + 1);
  string s;
  cin >> s;

  for (int i = 0; i < (1 << 20); ++i) {
    dp[i] = -1;
  }

  int val = 0;
  n = s.length();
  for (int i = 0; i < n; ++i) {
    if (s[i] == 'X') {
      val += (1 << i);
    }
  }

  printf("%.10lf\n", solve2(val));
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
