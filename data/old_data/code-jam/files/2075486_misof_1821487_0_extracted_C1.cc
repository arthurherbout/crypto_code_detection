// another fine solution by misof
// #includes {{{
#include <algorithm>
#include <numeric>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <stack>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <cassert>

#include <cmath>
#include <complex>
using namespace std;
// }}}

/////////////////// PRE-WRITTEN CODE FOLLOWS, LOOK DOWN FOR THE SOLUTION ////////////////////////////////

// pre-written code {{{
#define FOR(i,a,b) for(int i=(int)(a);i<=(int)(b);++i)
#define REP(i,n) for(int i=0;i<(int)(n);++i)
// }}}

/////////////////// CODE WRITTEN DURING THE COMPETITION FOLLOWS ////////////////////////////////

int N;
vector<int> X, Y;
vector<char> C;

bool check(int x, int y) {
    REP(n,N) {
        int d = max( abs( x-X[n] ), abs( y-Y[n] ) );
        if (d%2==1) if (C[n]=='.') return false;
        if (d%2==0) if (C[n]=='#') return false;
    }
    return true;
}

int main() {
    int T; cin >> T;
    FOR(t,1,T) {
        cin >> N;
        X.clear(); X.resize(N);
        Y.clear(); Y.resize(N);
        C.clear(); C.resize(N);
        REP(n,N) {scanf(" %d %d %c",&X[n],&Y[n],&C[n]);}
        bool found = false;
        int ansx=0, ansy=0;
        for (int s=0; s<=4000; ++s) {
            for (int x=-s; x<=+s; ++x) {
                int y;
                y = abs(x)-s;
                if (check(x,y)) {
                    found = true;
                    ansx = x;
                    ansy = y;
                }
                y = -y;
                if (check(x,y)) {
                    found = true;
                    ansx = x;
                    ansy = y;
                }
            }
            if (found) break;
        }
        if (found) {
            printf("Case #%d: %d %d\n",t,ansx,ansy);
        } else {
            printf("Case #%d: Too damaged\n",t);
        }
    }
}
// vim: fdm=marker:commentstring=\ \"\ %s:nowrap:autoread
