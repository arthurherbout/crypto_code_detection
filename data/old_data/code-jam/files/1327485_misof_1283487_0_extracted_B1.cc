// another fine solution by misof
// #includes {{{
#include <algorithm>
#include <numeric>

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <set>
#include <map>

#include <cstdio>
#include <cstdlib>
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

#include <cstring>

int A[22][22], WL[22][22], ES[22][22];
int R, C, M;
bool done[22][22];
int dr[]={-1,1,0,0}, dc[]={0,0,-1,1};

bool all_empty() {
    FOR(r,1,R) FOR(c,1,C) if (A[r][c]) return false;
    return true;
}

int main() {
    int T; cin >> T;
    FOR(t,1,T) {
        cin >> R >> C >> M;
        memset(A,0,sizeof(A));
        FOR(r,1,R) FOR(c,1,C) cin >> A[r][c];
        int days = 0;
        while (true) {
            if (all_empty()) break;
            ++days;

            // dijkstra to find water levels
            memset(WL,0,sizeof(WL));
            memset(done,0,sizeof(done));
            FOR(r,1,R) FOR(c,1,C) WL[r][c]=123456;
            for (int round=0; round<=(R+2)*(C+2); ++round) {
                int min=1234567, minr=-1, minc=-1;
                FOR(r,0,R+1) FOR(c,0,C+1) if (!done[r][c]) if (WL[r][c]<min) min=WL[r][c], minr=r, minc=c;
                done[minr][minc] = true;
                REP(d,4) {
                    int w=WL[minr][minc], nr=minr+dr[d], nc=minc+dc[d];
                    if (nr<0 || nr>R+1 || nc<0 || nc>C+1) continue;
                    if (WL[nr][nc] > max(w,A[nr][nc])) WL[nr][nc] = max(w,A[nr][nc]);
                }
            }

            // find erosion size
            FOR(r,1,R) FOR(c,1,C) {
                ES[r][c]=0;
                REP(d,4) {
                    int nr=r+dr[d], nc=c+dc[d];
                    if (WL[nr][nc] < WL[r][c]) ES[r][c] = max( ES[r][c], WL[r][c] - WL[nr][nc] );
                }
                if (ES[r][c] > M) ES[r][c] = M;
            }

            // erode
            FOR(r,1,R) FOR(c,1,C) A[r][c] -= ES[r][c];
        }
        printf("Case #%d: %d\n",t,days);
    }
}
// vim: fdm=marker:commentstring=\ \"\ %s:nowrap:autoread
