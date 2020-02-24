#include <stdio.h>

int main(void) {
  int T, t, R, C, W;
  scanf("%d", &T);
  for (t = 1; t <= T; ++t) {
    scanf("%d%d%d", &R, &C, &W);
    printf("Case #%d: %d\n", t, (C - 1) / W + W);
  }
  return 0;
}
