#include <cstdio>
#include <cstring>
int bb[55][55];
int main() {
  int Ti, Tn;
  scanf("%d", &Tn);
  for (Ti = 1; Ti <= Tn; ++Ti) {
    long long M, B;
    scanf("%lld %lld", &B, &M);
    memset(bb, 0, 55 * 55*sizeof(int));
    for (int i = B; i > 1 && M; --i) {
      bb[0][i - 1] = 1;
      bb[i - 1][B - 1] = 1;
      --M;
      long long v = 1;
      v <<= (B - i - 2);
      for (int j = i + 1; M && j < B; ++j, v >>= 1) {
        //printf("%d %lld, %lld\n", j, v, M);
        if (M >= v) {
          M -= v;
          bb[i - 1][j - 1] = 1;
        }
      }
    }
    bb[B - 1][B - 1] = 0;
    if (M > 0)
      goto np;
    printf("Case #%d: POSSIBLE\n", Ti);
    for (int i = 0; i < B; ++i) {
      for (int j = 0; j < B; ++j)
        printf("%d", bb[i][j]);
      printf("\n");
    }
    continue;
  np:
    printf("Case #%d: IMPOSSIBLE\n", Ti);
  }
}