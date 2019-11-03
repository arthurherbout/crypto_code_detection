#include <stdio.h>
#include <string.h>

int main(void) {
  int T, t, i, C, D, V, a[6], b[101], pattern, sum, j, ans;
  scanf("%d", &T);
  for (t = 1; t <= T; ++t) {
    scanf("%d%d%d", &C, &D, &V);
    for (i = 0; i < D; ++i)
      scanf("%d", &a[i]);
    memset(b, 0, (V + 1) * sizeof(int));
    for (pattern = 0; pattern < 1 << D; ++pattern) {
      sum = 0;
      for (i = 0; i < D; ++i)
        if (pattern & (1 << i))
          sum += a[i];
      if (sum <= V)
        b[sum] = 1;
    }
    ans = 0;
    for (i = 1; i <= V; ++i) {
      if (!b[i]) {
        ++ans;
        for (j = 1; j < i; ++j)
          if (b[j] && i + j <= V)
            b[i + j] = 1;
      }
    }
    printf("Case #%d: %d\n", t, ans);
  }
  return 0;
}
