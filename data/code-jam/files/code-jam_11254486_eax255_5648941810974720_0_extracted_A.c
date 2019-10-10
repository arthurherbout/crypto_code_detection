#include <stdio.h>
char res[2020];
const char *v[] = {"ZERO", "ONE", "TWO",   "THREE", "FOUR",
                   "FIVE", "SIX", "SEVEN", "EIGHT", "NINE"};
const char vc[] = "ZXSVFWGHOE";
const int va[] = {0, 6, 7, 5, 4, 2, 8, 3, 1, 9};
int main() {
  int Tn, Ti;
  scanf("%d", &Tn);
  for (Ti = 1; Ti <= Tn; ++Ti) {
    scanf("%s", res);
    int i = 0;
    int cnt[256] = {0};
    for (; res[i]; ++i)
      ++cnt[res[i]];
    int dcnt[10] = {0};
    for (i = 0; i < 10; ++i) {
      dcnt[va[i]] = cnt[vc[i]];
      int j = 0;
      for (; v[va[i]][j]; ++j)
        cnt[v[va[i]][j]] -= dcnt[va[i]];
    }
    int j = 0;
    for (i = 0; i < 10; ++i)
      while (dcnt[i]--)
        res[j++] = '0' + i;
    res[j] = 0;
    printf("Case #%d: %s\n", Ti, res);
  }
}