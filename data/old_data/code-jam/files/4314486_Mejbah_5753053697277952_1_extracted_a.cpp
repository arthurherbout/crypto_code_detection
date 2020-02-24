#include <bits/stdc++.h>

using namespace std;

int main()
{
  freopen("A-large.in", "r", stdin);
  freopen("A-large.out", "w", stdout);
  int t, n, s;
  pair<int, char> p[26];
  cin >> t;
  for( int cn = 1; cn <= t; cn++ ) {
    printf("Case #%d:", cn);
    cin >> n;
    s = 0;
    for( int i = 0; i < n; i++ ) {
      cin >> p[i].first;
      p[i].second = i+'A';
      s += p[i].first;
    }
    /*for( int i = 0; i < n; i++ ) {
      cout << p[i].first << ' ' << p[i].second << endl;
    }*/
    while(s) {
      sort(p, p+n);
      //for( int i = 0; i < n; i++ ) cout << p[i].first << ' ';
      if( p[n-2].first > 1 || s-2 != 1 /*( p[n-2].first == 1 && p[n-1].first == 1 )*/ ) {
        p[n-1].first--;
        p[n-2].first--;
        s -= 2;
        cout << ' ' << p[n-1].second<< p[n-2].second;
      } else {
        p[n-1].first--;
        s -= 1;
        cout << ' ' << p[n-1].second;
      }
    }
    puts("");
  }
}
