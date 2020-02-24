#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <string>
using namespace std;
#define N 100
/*
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
}*/

char expr[N];
int d;
int v[40][40];
long long f[40][250];

long long solve(char *expr, int d, int n)
{
  int i, j, k;
  
  for(i = 0; i < d; i++)
    for(j = i; j < d; j++)
      if(j == i) v[i][j] = (expr[i]-'0')%n;
      else v[i][j] = (v[i][j-1]*10+(expr[j]-'0'))%n;

  for(i = 0; i < d; i++)
    for(k = 0; k < n; k++) {
      f[i][k] = (v[0][i] % n == k);
      for(j = 0; j < i; j++)
        f[i][k] += f[j][(k+n-v[j+1][i])%n] + f[j][(k+v[j+1][i])%n];
    }
  //printf("(%s, %d) = %lld\n", expr, n, f[d-1][0]);
  return f[d-1][0];
}

int main()
{
  int t, index, i;
  long long r;
  scanf("%d", &t);
  for(index = 1; index <= t; index++) {
    scanf("%s", expr);
    d = strlen(expr);
//     r = solve(expr, d, 2)+solve(expr, d, 3)+solve(expr, d, 5)+solve(expr, d, 7);
//     r -= solve(expr, d, 6)+solve(expr, d, 10)+solve(expr, d, 14);
//     r -= solve(expr, d, 15)+solve(expr, d, 21)+solve(expr, d, 35);
//     r += solve(expr, d, 30)+solve(expr, d, 42)+solve(expr, d, 70)+solve(expr, d, 105);
//     r -= solve(expr, d, 210);
    r = solve(expr, d, 2)+solve(expr, d, 3) - solve(expr, d, 6);
    r = r + solve(expr, d, 5)-solve(expr, d, 10)-solve(expr, d, 15) + solve(expr, d, 30);
    r = r + solve(expr, d, 7)-solve(expr, d, 14) - solve(expr, d, 21)-solve(expr, d, 35);
    r = r + solve(expr, d, 42)+solve(expr, d, 70)+solve(expr, d, 105);
    r -= solve(expr, d, 210);
    printf("Case #%d: %lld\n", index, r);
  }
}

