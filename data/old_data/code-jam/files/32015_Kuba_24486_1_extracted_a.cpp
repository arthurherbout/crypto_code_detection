#include <map>
#include <set>
#include <cmath>
#include <queue>
#include <vector>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <numeric>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <ctime>
#include <functional>

using namespace std;



#define FOR( i, a, b )  for( int i =(a); i<=(b); ++i )
#define FORD( i, b, a ) for( int i =(b); i>=(a); --i )
#define CASE( i )       int __d; scanf("%d", &__d); FOR( i, 1, __d)
#define FOREACH( i, C ) for( __typeof(C.begin())i = C.begin(); i != C.end(); ++i )
#define ALL( V )        (V).begin(), (V).end()
#define SORT( V )       sort( ALL(V) )

int P, K, L;

int main()
{
    //ios::sync_with_stdio(false);
    long long TL[1002], S, tmp;
    
    CASE( N ) {
      FOR(i, 0, L) { TL[i] = 0; }
      
      S = 0;
      
      cin >> P >> K >> L;
      FOR( i, 0, L-1) {
        cin >> TL[i];
      }  
      
      sort(TL, TL + L);
      
     int j = 1, k = 1;     
     FORD(i, L-1, 0) {
        if ( (j == K) ) {
          S += k * TL[i];            
          ++k;
          j = 1;
        }else {        
            S += k * TL[i];
            ++j;
        }
     } 
          
          
      cout << "Case #" << N << ": " << S << endl;
      
    }
    
    return 0;
}

