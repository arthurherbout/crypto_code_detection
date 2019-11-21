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

int N ;
double V, C, R1, C1, R2, C2, T1, T2 ;

int main() {
    //ifstream Cin("input.txt");
    freopen("output.txt", "w", stdout);

    int _T, _NT ;
    cin >> _T ;
    for( _NT = 1 ; _NT <= _T ; ++_NT ) {
        cout << "Case #" << _NT << ": ";

        cin >> N >> V >> C;

        if (N == 1) {
            cin >> R1 >> C1;
            if (C1 != C) {
                cout << "IMPOSSIBLE" << endl;
                continue;
            }
            T1 = V / R1;
            printf("%.7f\n", T1);
        }
        else {
            cin >> R1 >> C1 >> R2 >> C2;

            if (C1 == C2) {
                if (C != C1)
                    cout << "IMPOSSIBLE" << endl;
                else {
                    T1 = V / (R1 + R2);
                    printf("%.7f\n", T1);
                }
                continue;
            }

            if (C2 < C1) {
                swap(C1, C2);
                swap(R1, R2);
            }

            if (C < C1 || C > C2) {
                cout << "IMPOSSIBLE" << endl;
                continue;
            }

            T1 = V / R1 * (C - C2) / (C1 - C2);
            T2 = V / R2 * (C - C1) / (C2 - C1);

            T1 = max(T1, T2);

            printf("%.7f\n", T1);
        }
    }
    return 0 ;
}