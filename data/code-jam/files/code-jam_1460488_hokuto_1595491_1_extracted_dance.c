#include <stdio.h>
typedef int bool;
#define true 1
#define false 0
//#define DEBUG

bool hasBest(int a, int b, int c) {
  for (int k = 0; k <= c; k++) {
    if (a >= b * 3 - k && a >= b) {
      return true;
    }
  }
  return false;
}

int main() {
  int t;
  scanf("%d", &t);
  for (int i = 0; i < t; i++) {
    int n, s, p;
    scanf("%d %d %d", &n, &s, &p);
#ifdef DEBUG
    printf("n:%d s:%d p:%d\n", n, s, p);
#endif
    int t[n];
    for (int j = 0; j < n; j++) {
      scanf("%d", (t + j));
    }
#ifdef DEBUG
    for (int j = 0; j < n; j++) {
      printf("t[%d] -> %d, ", j, t[j]);
    }
    printf("\n");
#endif
    int y = 0;

    int u[n];
    for (int j = 0; j < n; j++) {
      u[j] = -1;
    }
    for (int j = 0; j < n; j++) {
      if (!hasBest(t[j], p, 2)) {
        u[j] = t[j];
      } else {
        y++;
      }
    }
    for (int j = 0; j < n; j++) {
      if (u[j] != -1) {
        if (hasBest(u[j], p, 4) && s >= 1) {
          y++;
          s--;
        }
      }
    }
    printf("Case #%d: %d\n", i + 1, y);
  }
}
