#include <stdio.h>
char A[2020];
char B[2020];
char RA[2020];
char RB[2020];
int a[2020];
int b[2020];
int main() {
  int Tn, Ti;
  scanf("%d", &Tn);
  for (Ti = 1; Ti <= Tn; ++Ti) {
    scanf("%s", A);
    scanf("%s", B);
    int i = 0;
    int cmp = 0;
    int ka = -1;
    int kb = -1;
    for (i = 0; A[i]; ++i) {
      if (A[i] == '?' && B[i] == '?') {
        if (cmp == 0)
          RA[i] = RB[i] = '0';
        else if (cmp < 0)
          RA[i] = '9', RB[i] = '0';
        else if (cmp > 0)
          RA[i] = '0', RB[i] = '9';
      } else if (A[i] == '?') {
        RB[i] = B[i];
        if (cmp == 0)
          RA[i] = RB[i], ka = i;
        else if (cmp < 0)
          RA[i] = '9';
        else if (cmp > 0)
          RA[i] = '0';
      } else if (B[i] == '?') {
        RA[i] = A[i];
        if (cmp == 0)
          RB[i] = RA[i], kb = i;
        else if (cmp < 0)
          RB[i] = '0';
        else if (cmp > 0)
          RB[i] = '9';
      } else {
        RA[i] = A[i];
        RB[i] = B[i];
        if (cmp == 0) {
          if (RB[i] > RA[i])
            cmp = -1;
          else if (RB[i] < RA[i])
            cmp = 1;
        }
      }
    }
    RA[i] = RB[i] = 0;
    long long ac[3], bc[3], dc[3][3];
    ac[1] = bc[1] = 0;
    for (i = 0; A[i]; ++i)
      ac[1] *= 10, ac[1] += RA[i] - '0', bc[1] *= 10, bc[1] += RB[i] - '0';
    ac[0] = ac[2] = ac[1], bc[0] = bc[2] = bc[1];
    if (ka > -1) {
      if (RA[ka] > '0') {
        RA[ka]--;
        ac[0] = 0;
        for (i = 0; A[i]; ++i)
          ac[0] *= 10, ac[0] += RA[i] - '0';
        RA[ka]++;
      }
      if (RA[ka] < '9') {
        RA[ka]++;
        ac[2] = 0;
        for (i = 0; A[i]; ++i)
          ac[2] *= 10, ac[2] += RA[i] - '0';
        RA[ka]--;
      }
    }
    if (kb > -1) {
      if (RB[kb] > '0') {
        RB[kb]--;
        bc[0] = 0;
        for (i = 0; A[i]; ++i)
          bc[0] *= 10, bc[0] += RB[i] - '0';
        RB[kb]++;
      }
      if (RB[kb] < '9') {
        RB[kb]++;
        bc[2] = 0;
        for (i = 0; A[i]; ++i)
          bc[2] *= 10, bc[2] += RB[i] - '0';
        RB[kb]--;
      }
    }
#define ABS(a) (((a) < 0) ? -(a) : (a))
    dc[0][0] = ABS(ac[0] - bc[0]);
    dc[0][1] = ABS(ac[0] - bc[1]);
    dc[0][2] = ABS(ac[0] - bc[2]);
    dc[1][0] = ABS(ac[1] - bc[0]);
    dc[1][1] = ABS(ac[1] - bc[1]);
    dc[1][2] = ABS(ac[1] - bc[2]);
    dc[2][0] = ABS(ac[2] - bc[0]);
    dc[2][1] = ABS(ac[2] - bc[1]);
    dc[2][2] = ABS(ac[2] - bc[2]);
    int ai = 0, bi = 0;
    if (dc[ai][bi] > dc[0][1])
      ai = 0, bi = 1;
    if (dc[ai][bi] > dc[0][2])
      ai = 0, bi = 2;
    if (dc[ai][bi] > dc[1][0])
      ai = 1, bi = 0;
    if (dc[ai][bi] > dc[1][1])
      ai = 1, bi = 1;
    if (dc[ai][bi] > dc[1][2])
      ai = 1, bi = 2;
    if (dc[ai][bi] > dc[2][0])
      ai = 2, bi = 0;
    if (dc[ai][bi] > dc[2][1])
      ai = 2, bi = 1;
    if (dc[ai][bi] > dc[2][2])
      ai = 2, bi = 2;
    if (ai == 0 && RA[ka] > '0')
      RA[ka]--;
    if (ai == 2 && RA[ka] < '9')
      RA[ka]++;
    if (bi == 0 && RB[kb] > '0')
      RB[kb]--;
    if (bi == 2 && RB[kb] < '9')
      RB[kb]++;
    printf("Case #%d: %s %s\n", Ti, RA, RB);
  }
}