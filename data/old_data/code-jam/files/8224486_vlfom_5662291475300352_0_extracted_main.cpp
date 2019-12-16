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

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define FOR0(i,n) for( i = 0 ; i < n ; ++i )
#define FOR1(i,n) for( i = 1 ; i <= n ; ++i )
#define sys_p system( "pause" )
#define pb push_back
#define mp make_pair
#define FI first
#define SE second
#define sz(a) (int)a.size()

typedef long long LL ;

int main() {
    freopen("input.txt", "r", stdin) ;
    freopen("output.txt", "w", stdout) ;

    int n, i ;
    double d, h, m, d1, m1, d2, m2 ;
    double tend, dend, v1, v2, dv, dd, dt ;

    int T, _T ;
    cin >> T ;
    for( _T = 1 ; _T <= T ; ++_T ) {
        cout << "Case #" << _T << ": ";

        cin >> n;
        if (n == 1) {
            cin >> d >> h >> m;
            if( h == 1 )
                cout << 0 << endl;
            else {
                v1 = 360.0 / (m+1) ;
                v2 = 360.0 / m ;
                tend = (360-d) / v1 ;
                if( v2 * tend >= 360-d + 360 )
                    cout << 1 << endl ;
                else
                    cout << 0 << endl ;
            }
        }
        else {
            cin >> d1 >> h >> m1;
            cin >> d2 >> h >> m2;
            if (d1 > d2) {
                swap(d1, d2);
                swap(m1, m2);
            }
            v1 = 360.0 / m1;
            v2 = 360.0 / m2;
            if (m1 == m2)
                cout << 0 << endl;
            else if (v2 > v1) {
                tend = (360 - d2) / v2;
                d1 += tend * v1;
                if (d1 >= 360)
                    cout << 0 << endl;
                else {
                    dd = 360 - d1;
                    tend = dd / v1 ;
                    if ( v2 * tend >= 360 )
                        cout << 1 << endl ;
                    else
                        cout << 0 << endl ;
                }
            }
            else {
                dv = v1 - v2;
                dd = d2 - d1;
                dt = dd / dv;
                if (d2 + dt * v2 >= 360)
                    cout << 0 << endl;
                else {
                    d2 += dt * v2 ;
                    d1 = d2 ;

                    swap( d1, d2 ) ;
                    swap( v1, v2 ) ;

                    tend = (360 - d2) / v2;
                    d1 += tend * v1;
                    if (d1 >= 360)
                        cout << 0 << endl;
                    else {
                        dd = 360 - d1;
                        tend = dd / v1 ;
                        if ( v2 * tend >= 360 )
                            cout << 1 << endl ;
                        else
                            cout << 0 << endl ;
                    }
                }
            }
        }
    }
}