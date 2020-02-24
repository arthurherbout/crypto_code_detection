#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <string>
using namespace std;
#define N 100

char expr[N];
int d;
int pow3[13];

long long calculate(char *expr, int d, int m)
{
  long long s = 0, r = expr[0]-'0', op = 1;
  for(int i = 1; i < d; i++, m/=3)
    if(m % 3 == 1) {
      s += r * op;
      r = expr[i] - '0';
      op = 1;
    } else if(m % 3 == 2) {
      s += r * op;
      r = expr[i] - '0';
      op = -1;
    } else {
      r = r * 10 + (expr[i]-'0');
    }
  s += r * op;
//  printf("%lld\n", s);
  return s;
}

int main()
{
  int t, index, i, r;
  pow3[0] = 1;
  for(i = 1; i < 13; i++)
    pow3[i] = pow3[i-1]*3;
  scanf("%d", &t);
  for(index = 1; index <= t; index++) {
    scanf("%s", expr);
    d = strlen(expr);
    r = 0;
    for(i = 0; i < pow3[d-1]; i++) {
      long long sum = calculate(expr, d, i);
      if(sum % 2 == 0 || sum % 3 == 0 || sum % 5 == 0 || sum % 7 == 0)
        r++;
    }
    printf("Case #%d: %d\n", index, r);
  }
}
