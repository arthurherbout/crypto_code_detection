#include <bits/stdc++.h>

using namespace std;

int main()
{
  freopen("B-large.in", "r", stdin);
  freopen("B-large.out", "w", stdout);
  int t, a, l, i;
  char c, s[103];

  scanf("%d", &t);
  for( int cn = 1; cn <= t; cn++ ) {
    scanf("%s", s);
    for( l = 0; s[l] != '\0'; l++ );
    while( s[l-1] == '+' ) l--;
    s[l] == '\0';
    a = 0;
    while(l) {
      a++;
      c = s[0];
      s[0] = c == '+' ? '-' : '+';
      i = 1;
      while( s[i] == c ) {
        s[i] = s[0];
        i++;
      }
      if( i == l ) {
        if( s[0] == '-' ) a++;
        break;
      }
    }
    printf("Case #%d: %d\n", cn, a);
  }
}
