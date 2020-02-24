#include <stdio.h>
#include <algorithm>

#define INF 1000000000000000ll
#define MOD 1000000007

int dist(int x1, int y1, int x2, int y2) {
  return std::abs(x1 - x2) + std::abs(y1 - y2);
}

int main() {
  int T;
  int R, C, N;
  int r[201], c[201];
  long long D, b[201], lft, rgt, sum;
  bool fail;
  scanf("%d", &T);
  for (int t = 1; t <= T; ++t) {
    scanf("%d%d%d%lld", &R, &C, &N, &D);
    for (int i = 0; i < N; ++i)
      scanf("%d%d%lld", &r[i], &c[i], &b[i]);
    sum = 0;
    fail = false;
    for (int i = 1; i <= R; ++i) {
      for (int j = 1; j <= C; ++j) {
        lft = 1;
        rgt = INF;
        for (int k = 0; k < N; ++k) {
          lft = std::max(lft, b[k] - D * dist(i, j, r[k], c[k]));
          rgt = std::min(rgt, b[k] + D * dist(i, j, r[k], c[k]));
        }
        if (lft <= rgt)
          sum += rgt;
        else
          fail = true;
      }
    }
    printf("Case #%d: ", t);
    if (fail) printf("IMPOSSIBLE\n");
    else printf("%lld\n", sum % MOD);
  }
  return 0;
}
