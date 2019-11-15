#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;

int S, k;
char buf[55000];
int p[16];

int go() {
  int ret = 0;
  char last = ' ';
  for (int i = 0; i < S; i += k) {
    for (int j = 0; j < k; j++) {
      char c = buf[i+p[j]];
      if (c == last) continue;
      last = buf[i+p[j]];
      ret++;
    }
  }
  return ret;
}

int main() {
  int N; scanf("%d", &N);
  for (int it = 1; it <= N; it++) {
    scanf("%d", &k);
    scanf("%s", &buf);
    S = strlen(buf);

    for (int i = 0; i < k; i++) p[i] = i;

    int ret = 1000000;
    do {
      ret <?= go();
    } while (next_permutation(p, p+k));
    printf("Case #%d: %d\n", it, ret);
  }
}
