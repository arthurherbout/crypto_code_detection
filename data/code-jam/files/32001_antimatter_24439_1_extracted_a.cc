#include <cstdio>
#include <cstring>
using namespace std;

struct node {
  int a;  // 1 = and, 0 = or
  bool c;  // changeable?
} N[50000];

int save[50000][2];
int M, V;
int bad;

int go(int n, int val) {
  if (save[n][val] != bad) {
    return save[n][val];
  }

  if (N[n].a >= 100) {
    // leaf
    int v = N[n].a - 100;
    if (v == val) return save[n][val] = 0;
    else return save[n][val] = bad;
  }
  else {
    // interior

    int ret = 1000000000;
    bool a = (N[n].a == 1);
    bool c = N[n].c;
    int cc = 1;
    if (!c) cc = bad;

    if (val == 1) {
      // we want 1
      ret <?= go(2*n, 1) + go(2*n+1, 1) + (a?0:cc);   //and
      ret <?= go(2*n, 0) + go(2*n+1, 1) + (a?cc:0);   //or  0 1
      ret <?= go(2*n, 1) + go(2*n+1, 0) + (a?cc:0);   //or  1 0
      ret <?= go(2*n, 1) + go(2*n+1, 1) + (a?cc:0);   //or  1 1 
   }
    else {
      // we want 0
      ret <?= go(2*n, 0) + go(2*n+1, 0) + (a?0:cc);   //and 0 0
      ret <?= go(2*n, 0) + go(2*n+1, 1) + (a?0:cc);   //and 0 1
      ret <?= go(2*n, 1) + go(2*n+1, 0) + (a?0:cc);   //and 1 0
      ret <?= go(2*n, 0) + go(2*n+1, 0) + (a?cc:0);   //or  0 0
    }
    return save[n][val] = ret;
  }
}

int main() {
  int NNN; scanf("%d", &NNN);
  for (int it = 1; it <= NNN; it++) {
    memset(save, 1, sizeof(save));
    bad = save[0][0];

    scanf("%d %d", &M, &V);
    for (int i = 0; i < (M-1) / 2; i++) {
      int G, C;
      scanf("%d %d", &G, &C);
      N[i+1].a = G;
      N[i+1].c = C;
    }
    int offset = (M+1)/2;
    for (int i = 0; i < (M+1) / 2; i++) {
      int k; scanf("%d", &k);
      N[i+offset].a = k+100;
      N[i+offset].c = 0;
    }

    int ret = go(1, V);
    printf("Case #%d: ", it);
    if (ret >= bad) printf("IMPOSSIBLE\n");
    else printf("%d\n", ret);
  }
}

