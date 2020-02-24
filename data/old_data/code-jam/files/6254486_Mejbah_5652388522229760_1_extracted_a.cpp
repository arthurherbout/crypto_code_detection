#include <bits/stdc++.h>

using namespace std;

int main()
{
  freopen("A-large.in", "r", stdin);
  freopen("ou.txt", "w", stdout);

  int t, i, n, m;
  set<int> s;

  scanf("%d", &t);
  for( int cn = 1; cn <= t; cn++ ) {
    scanf("%d", &n);
    if( n == 0 ) {
      printf("Case #%d: INSOMNIA\n", cn);
      continue;
    }
    for( i = 0; i < 10; i++ )
      s.insert(i);
    for( i = 1; ; i++ ) {
      m = i*n;
      while(m) {
        s.erase(m%10);
        m /= 10;
      }
      if(s.empty()) break;
    }
    printf("Case #%d: %d\n", cn, i*n);
  }
}
