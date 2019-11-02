#include <stdio.h>
#include <stdint.h>
uint64_t c[11];
uint64_t b[11];
uint64_t cnv(uint64_t j, uint64_t bb) {
  uint64_t k = 0;
  uint64_t l = 1;
  uint64_t ll = 1;
  while (j) {
    if (j & ll)
      k += l, j ^= ll;
    l *= bb;
    ll <<= 1;
  }
  return k;
}
uint64_t aluku(uint64_t a) {
  if (a <= 2)
    return 0;
  if (~a & 1)
    return 2;
  for (uint64_t i = 3; (i * i) < a; i += 2) {
    if ((a % i) == 0)
      return i;
  }
  return 0;
}
int main() {
  int T, Ti;
  uint64_t N, J, i;
  scanf("%d", &T);
  for (Ti = 1; Ti <= T; ++Ti) {
    scanf("%lu %lu", &N, &J);
    printf("Case #%d:\n", Ti);
    int n = 1 << (N - 2);
    int nn = 1 << (N - 1);
    for (i = n; i < nn && J; ++i) {
      int v = (i << 1) | 1;
      for (int j = 2; j <= 10; ++j) {
        b[j] = cnv(v, j);
        c[j] = aluku(b[j]);
        if (!c[j])
          goto end;
      }
      printf("%lu %lu %lu %lu %lu %lu %lu %lu %lu %lu\n", b[10], c[2], c[3],
             c[4], c[5], c[6], c[7], c[8], c[9], c[10]);
      --J;
    end:
      continue;
    }
  }
  return 0;
}