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

int N, K;
vector<int> A, B, C;
vector<double> P;

int main() {
    int T; scanf("%d ",&T);
    FOR(t,1,T) {
        scanf("%d ",&N);
        scanf("%d ",&K);
        A.clear(); A.resize(N);
        B.clear(); B.resize(N);
        C.clear(); C.resize(N);
        REP(n,N) {
            scanf("%d/%d %d ",&A[n],&B[n],&C[n]);
        }
        P.clear();
        REP(n,N) REP(i,C[n]) P.push_back((1.*A[n])/B[n]);
        sort(P.begin(),P.end());
        reverse(P.begin(),P.end());
        double best = 0.;
        for (int big=0; big<=K; ++big) {
            vector<double> used;
            used.insert( used.end(), P.begin(), P.begin()+big );
            used.insert( used.end(), P.end()-(K-big), P.end() );
            double awake = 1., asleep = 0., bad = 0.;
            for (int k=0; k<K; ++k) {
                double nawake = awake * used[k];
                double nasleep = (awake+asleep) * (1-used[k]);
                double nbad = bad + asleep * used[k];
                awake = nawake;
                asleep = nasleep;
                bad = nbad;
            }
            double good = 1-bad;
            best = max(best,good);
        }
        printf("Case #%d: %.10f\n",t,1.-best);
    }
}
// vim: fdm=marker:commentstring=\ \"\ %s:nowrap:autoread
