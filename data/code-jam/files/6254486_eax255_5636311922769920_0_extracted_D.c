#include <stdio.h>
int main() {
  int T, Ti;
  int K, C, S, i;
  scanf("%d", &T);
  for (Ti = 1; Ti <= T; ++Ti) {
    scanf("%d %d %d", &K, &C, &S);
    printf("Case #%d:", Ti);
    for (i = 0; i < S; ++i)
      printf(" %d", i + 1);
    printf("\n");
  }
  return 0;
}