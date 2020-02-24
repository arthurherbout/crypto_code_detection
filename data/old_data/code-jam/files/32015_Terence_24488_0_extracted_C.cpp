#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <string>
using namespace std;
#define N 500000
#define M 1000000007

int n, a[N];
long long s[N], r, A[N];

void generate(long long A[], int m, int n, long long X, long long Y, long long Z)
{
  for (int i = 0, j=0; i < n; i++, j++) {
    if (j == m) j = 0;
    a[i] = (int)A[j];
    A[j] = (X * A[j] + Y * (i + 1)) % Z;
  }
}

int main()
{
  int t, index, i, j;
  int m;
  long long X, Y, Z;
  
  scanf("%d", &t);
  for(index = 1; index <= t; index++) {
    scanf("%d%d%lld%lld%lld", &n, &m, &X, &Y, &Z);
    for(i = 0; i < m; i++)
      scanf("%lld", &A[i]);
    generate(A, m, n, X, Y, Z);
    for(i = 0; i<n; i++) {
      s[i] = 1;
      for(j = 0; j < i; j++)
        if(a[j] < a[i]) {
          s[i] += s[j];
        }
      s[i] %= M;
    }
    for(i = 0, r=0; i < n; i++)
      r += s[i];
    r %= M;
    printf("Case #%d: %lld\n", index, r);
  }
}
