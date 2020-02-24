#include <bits/stdc++.h>

using namespace std;

long long base[11], print[11], p[11][17];
bool isPrime[33333335];
vector<int> v;

void power() {
  for( int i = 2; i <= 10; i++ )
    p[i][0] = 1;
  for( int i = 2; i <= 10; i++ )
    for( int j = 1; j < 16; j++ )
      p[i][j] = p[i][j-1] * i;
}

void prime() {
  long long b = 1111111111111111;
  long long a = 33333334;
  isPrime[0] = isPrime[1] = 1;
  for( int i = 4; i <= a; i += 2 ) isPrime[i] = 1;
  for( int i = 3; i*i <= a; i += 2 )
    if(!isPrime[i])
      for( int j = i*i; j <= a; j += i )
        isPrime[j] = 1;
  for( int i = 0; i < a; i++ )
    if(!isPrime[i])
      v.push_back(i);
}

void base_comvert( int m ) {
  for( int i = 2; i <= 10; i++ )
    base[i] = 1;
  m >>= 1;
  int j = 1;
  while(m) {
    for( int i = 2; i <= 10; i++ )
      base[i] += (m&1) * p[i][j];
    m >>= 1;
    j++;
  }
}

bool is_prime( int id, long long n ) {
  for( int i = 0; i < v.size(); i++ )
    if( v[i]*v[i] <= n ) {
      if( n%v[i] == 0 ) {
        print[id] = v[i];
        return 0;
      }
    }
    else return 1;
  return 1;
}

int main()
{
  freopen("C-small-attempt0.in", "r", stdin);
  freopen("C-small-attempt0.out", "w", stdout);
  int n, j, t, m;
  power();
  prime();
  scanf("%d", &t);
  for( int cn = 1; cn <= t; cn++ ) {
    scanf("%d%d", &n, &j);
    printf("Case #%d:\n", cn);
    m = (1 << n-1) + 1;
    for( int l = m, k = 0; l < (1<<n); l += 2 ) {
      base_comvert(l);
      int x = 0;
      for( int i = 2; i <= 10; i++ ) {
        if(is_prime(i, base[i])) break;
        else x++;
      }
      if( x == 9 ) {
        printf("%lld", base[10]);
        for( int i = 2; i <= 10; i++ ) printf(" %lld", print[i]);
        printf("\n");
        k++;
      }
      if( k == j ) break;
    }
  }
}
