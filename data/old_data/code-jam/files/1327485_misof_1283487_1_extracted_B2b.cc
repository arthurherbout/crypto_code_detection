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

int R, C;
long long M;
bool done[22][22];
int dr[]={-1,1,0,0}, dc[]={0,0,-1,1};

bool all_empty(const vector< vector<long long> > &A) {
    FOR(r,1,R) FOR(c,1,C) if (A[r][c]) return false;
    return true;
}

vector< vector<long long> > get_single_erosion(const vector< vector<long long> > &A) {
    vector< vector<long long> > WL(R+2, vector<long long>(C+2,0));
    vector< vector<long long> > ES(R+2, vector<long long>(C+2,0));
    vector< vector<bool> > done(R+2, vector<bool>(C+2,false));
    
    // dijkstra to find water levels
    FOR(r,1,R) FOR(c,1,C) WL[r][c]=1234567890123456LL;
    for (int round=0; round<(R+2)*(C+2); ++round) {
        long long min=1234567890123457LL;
        int minr=-1, minc=-1;
        FOR(r,0,R+1) FOR(c,0,C+1) if (!done[r][c]) if (WL[r][c]<min) min=WL[r][c], minr=r, minc=c;
        assert(minr!=-1);
        done[minr][minc] = true;
        REP(d,4) {
            long long w=WL[minr][minc];
            int nr=minr+dr[d], nc=minc+dc[d];
            if (nr<0 || nr>R+1 || nc<0 || nc>C+1) continue;
            if (WL[nr][nc] > max(w,A[nr][nc])) WL[nr][nc] = max(w,A[nr][nc]);
        }
    }
    
    // find erosion size (not limited to M)
    FOR(r,1,R) FOR(c,1,C) {
        ES[r][c]=0;
        REP(d,4) {
            int nr=r+dr[d], nc=c+dc[d];
            if (WL[nr][nc] < WL[r][c]) ES[r][c] = max( ES[r][c], WL[r][c] - WL[nr][nc] );
        }
        ES[r][c] = min( ES[r][c], M );
    }
    return ES;
}

void make_single_erosion(vector< vector<long long> > &A) {
    vector< vector<long long> > ES = get_single_erosion(A);
    FOR(r,1,R) FOR(c,1,C) A[r][c] -= ES[r][c];
}

int main() {
    int ADD; cin >> ADD;
    int T; cin >> T;
    FOR(t,1,T) {
        cin >> R >> C >> M;
        vector< vector<long long> > A(R+2, vector<long long>(C+2,0));
        FOR(r,1,R) FOR(c,1,C) cin >> A[r][c];
        long long days = 0;
        while (true) {
            if (all_empty(A)) break;
            vector< vector<long long> > ES = get_single_erosion(A);
            bool local = false;
            FOR(r,1,R) FOR(c,1,C) if (ES[r][c]>0 && ES[r][c]<M) local = true;
            if (local) {
                ++days;
                FOR(r,1,R) FOR(c,1,C) A[r][c] -= ES[r][c];
            } else {
                long long d=1; 
                long long subtract = M;
                while (true) {
                    d *= 2;
                    subtract *= 2;
                    vector< vector<long long> > B(A);
                    FOR(r,1,R) FOR(c,1,C) if (ES[r][c]==M) B[r][c]-=subtract;
                    bool ok=true;
                    FOR(r,1,R) FOR(c,1,C) if (B[r][c]<0) { ok=false; break; }
                    if (ok) FOR(r,1,R) if (ok) FOR(c,1,C) if (ok) REP(d,4) {
                        int nr=r+dr[d], nc=c+dc[d];
                        if ((A[r][c] > A[nr][nc]) != (B[r][c] > B[nr][nc])) { ok=false; break; }
                        if ((A[r][c] < A[nr][nc]) != (B[r][c] < B[nr][nc])) { ok=false; break; }
                    }
                    vector< vector<long long> > ES2 = get_single_erosion(B);
                    if (ES != ES2) ok = false;
                    if (!ok) break;
                }
                d /= 2;
                subtract /= 2;
                days += d;
                // cerr << "adding " << d << " days at once" << endl;
                FOR(r,1,R) FOR(c,1,C) if (ES[r][c]==M) A[r][c]-=subtract;
            }
        }
        printf("Case #%d: %lld\n",t+ADD,days);
    }
}
// vim: fdm=marker:commentstring=\ \"\ %s:nowrap:autoread
