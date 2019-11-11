#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <bitset>
#include <set>
#include <map>
#include <vector>
#include <string>
#include <cstring>
#include <queue>
#include <memory.h>
#include <cmath>
using namespace std;
 
#define max(a,b) ((a)>(b)?(a):(b))
#define FOR0(i,n) for( i = 0 ; i < n ; ++i )
#define FOR1(i,n) for( i = 1 ; i <= n ; ++i )
#define sys_p system( "pause" )
#define pb push_back
#define mp make_pair
#define FI first
#define SE second
 
typedef long long LL ;
const LL INF = (LL)1e18 ;
const LL MOD = (LL)1e9 + 7 ;

LL t, T, n, m, k, a, b, ans ;
     
main()
{       
     freopen( "input.txt", "r", stdin ) ;
     freopen( "output.txt", "w", stdout ) ;
     
     int i, j, c, nn ;
     cin >> T ;
     for( t = 1 ; t <= T ; ++t )
     {
          cin >> n >> m >> k ;
          
          if( k == 1 )
            ans = 1 ;
          else if( n <= 2|| m <= 2 )
            ans = k ;
          else
          {
            ans = min( k, 2*(n+m-2) ) ;
            for(a=3;a<=n;++a)for(b=3;b<=m;++b)
            {
              if( a*b-4 >= k )
                ans = min( ans, 2*(a+b-4) ) ;
              if( a*b-4 + 1 >= k )
                ans = min( ans, 2*(a+b-4) + 1 ) ;
              if( a*b-4 + 2 >= k )
                ans = min( ans, 2*(a+b-4) + 2 ) ;
              if( a*b-4 + 3 >= k )
                ans = min( ans, 2*(a+b-4) + 3 ) ;
              if( a*b-4 + 4 >= k )
                ans = min( ans, 2*(a+b-4) + 4 ) ;
            }
          }
          
          cout << "Case #" << t << ": " << ans << endl ;
     } 
} 
