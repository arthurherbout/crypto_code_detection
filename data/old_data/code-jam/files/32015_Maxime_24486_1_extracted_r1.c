#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
  int num;
  long long freq;
} FREQ;


int cmpF(const void *v1, const void *v2) {
  FREQ *e1 = (FREQ*)v1, *e2 = (FREQ*)v2;
  if (e1->freq < e2->freq) return 1;
  if (e1->freq > e2->freq) return -1;
  return 0;
}


main() {
  char str[4096];
  int N, P, K, L;
  int iN, i, impossible, level;
  long long result;

  FREQ F[1024];

  fgets(str, sizeof(str), stdin);
  sscanf(str, "%d", &N);
  fprintf(stderr, "N:%d\n", N);

  for (iN = 0; iN < N; iN++) {

    result = 0;
    bzero(&F, sizeof(F));

    fscanf(stdin, "%d %d %d", &P, &K, &L);
    fprintf(stderr, "P: %d K: %d  L:%d   K*P:%d\n", P, K, L, K * P);
 
 
    for(i = 0; i < L; i++) {
      fscanf(stdin, "%lld", &F[i].freq);
      F[i].num = i;
    }
    qsort(F, (size_t)L, sizeof(FREQ), &cmpF);
 
    for(i = 0; i < L; i++) {
      level = (int)i / K + 1;
      result += (long long)F[i].freq * (long long) level;
      fprintf(stderr, "i:%d  freq:%lld  level: %d\n", i, F[i].freq, level);
    }
    fprintf(stdout, "Case #%d: %lld\n", iN + 1, result);

  }
  return 0;
}
