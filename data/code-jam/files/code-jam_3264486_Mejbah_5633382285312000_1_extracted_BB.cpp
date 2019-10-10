#include <bits/stdc++.h>

using namespace std;

typedef long long int LL;

bool isTidy(LL n) {
  vector<int> v;
  while (n) {
    v.push_back(n%10);
    n /= 10;
  }
  for (int i = (int)v.size() - 1; i > 0; i--) {
    if (v[i] > v[i - 1]) {
      return 0;
    }
  }
  return 1;
}

int ithDigit(int i, LL n) {
  vector<int> v;
  while (n) {
    v.push_back(n%10);
    n /= 10;
  }
  for (int l = (int)v.size() - 1, j = 1; l >= 0; l--, j++) {
    if (j == i) {
      return v[l];
    }
  }
  return -1;
}

int numberOfDigit(LL n) {
  int d = 0;
  while (n) {
    d++;
    n /= 10;
  }
  return d;
}

LL change(int i, int d, LL n) {
  vector<int> v;
  while (n) {
    v.push_back(n%10);
    n /= 10;
  }
  LL r = 0;
  for (int l = (int)v.size() - 1, j = 1; l >= 0; l--, j++) {
    if (j == i) {
      v[l] = d;
    }
    r = 10 * r + v[l];
  }
  return r;
}

int main() {
  int T;
  scanf("%d", &T);
  for (int cn = 1; cn <= T; cn++) {
    LL N;
    scanf("%lld", &N);
    if (isTidy(N)) {
      printf("Case #%d: %lld\n", cn, N);
      continue;
    }
    // Number of digit
    int d = numberOfDigit(N);
    LL l = 0, r = 0;
    for (int i = 1; i < d; i++) {
      l = 10 * l + 9;
    }
    for (int i = 0; i < d; i++) {
      r = 10 * r + 1;
    }
    LL ans;
    if (N >= l && N < r) {
      ans = l;
    } else {
      ans = N;
      for (int i = 2; i <= d; i++) {
        int x = ithDigit(i - 1, ans);
        int y = ithDigit(i, ans);
        if (y < x) {
          ans = change(i - 1, x - 1, ans);
          for (int j = i; j <= d; j++) {
            ans = change(j, 9, ans);
          }
          i = 1;
        }
      }
    }
    printf("Case #%d: %lld\n", cn, ans);
  }
}
