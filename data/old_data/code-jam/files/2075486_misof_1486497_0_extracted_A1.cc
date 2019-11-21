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

struct zombie {
    int x,y,m;
    zombie(int x, int y, int m) : x(x), y(y), m(m) { }
};

bool operator< (const zombie &A, const zombie &B) { return A.m < B.m; }

#define INF 1234567890

int best[110][110];
int dist[110][110];

int zdist(const zombie &A, const zombie &B) { return 100*max(abs(B.x-A.x),abs(B.y-A.y)); }

int main() {
    int T; cin >> T;
    FOR(t,1,T) {
        int N; cin >> N;
        vector<zombie> Z;
        REP(n,N) { int x,y,m; cin >> x >> y >> m; Z.push_back(zombie(x,y,m)); }
        sort(Z.begin(),Z.end());
        vector<int> P(N);
        REP(n,N) P[n] = n;
        int ans = 0;
        do {
            int cas = 0, refresh = 0, x = 0, y = 0, zabil = 0;
            REP(n,N) {
                int z = P[n];
                int travel = 100*max( abs(x-Z[z].x), abs(y-Z[z].y) );
                int arrive = cas+travel;
                if (arrive < refresh) arrive = refresh;
                if (arrive < Z[z].m) arrive = Z[z].m;
                if (arrive > Z[z].m+1000) break;
                ++zabil;
                cas = arrive;
                refresh = arrive+750;
                x = Z[z].x;
                y = Z[z].y;
            }
            if (zabil > ans) ans = zabil;
        } while (next_permutation(P.begin(),P.end()));
        printf("Case #%d: %d\n",t,ans);
    }
}
// vim: fdm=marker:commentstring=\ \"\ %s:nowrap:autoread
