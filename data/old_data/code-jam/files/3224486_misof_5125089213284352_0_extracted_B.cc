// another fine solution by misof
// #includes {{{
#include <algorithm>
#include <iostream>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include <cassert>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <cmath>
#include <complex>
using namespace std;
// }}}

/////////////////// PRE-WRITTEN CODE FOLLOWS, LOOK DOWN FOR THE SOLUTION ////////////////////////////////

// pre-written code {{{
#define FOR(i,a,b) for(int i=(int)(a);i<=(int)(b);++i)
// GETTIMER machinery {{{"
#include <sys/time.h>
struct timeval __timer1, __timer2;
bool __timerInitialized = false;
#define STARTTIMER { gettimeofday(&__timer1,NULL); __timerInitialized = true; }
#define GETTIMER ({ assert(__timerInitialized); gettimeofday(&__timer2,NULL); long long __runtime = 1000000LL*(__timer2.tv_sec-__timer1.tv_sec) + 1LL*(__timer2.tv_usec-__timer1.tv_usec); __runtime; })
// }}}
#define REP(i,n) for(int i=0;i<(int)(n);++i)
#define SIZE(t) ((int)((t).size()))
// }}}

/////////////////// CODE WRITTEN DURING THE COMPETITION FOLLOWS ////////////////////////////////

void simulate(const vector<int> &prereq, vector<int> depend, const vector<int> &subtreesize, const vector<double> &subtreeweight, const string &names, const vector<string> &cool, vector<int> &counts) {
    int N = SIZE(prereq), M = SIZE(cool);
    vector<bool> done(N,false);
    string vygeneroval = "";
    REP(kolo,N) {
        // vyberieme nahodne dalsi prvok
        vector<double> pp(N,0.);
        REP(n,N) if (!done[n]) if (prereq[n]==-1 || done[prereq[n]]) pp[n] = subtreesize[n];
        double total = 0;
        REP(n,N) total += pp[n];
        REP(n,N) pp[n] /= total;
        double gen = (rand()*1.) / RAND_MAX;
        int kto = 0;
        while (pp[kto] < gen) { gen -= pp[kto]; ++kto; }
        done[kto] = true;
        vygeneroval += names[kto];
    }
    REP(m,M) if (vygeneroval.find(cool[m]) != string::npos) ++counts[m];
}

int main() {
    int T; cin >> T;
    FOR(test,1,T) {
        int N; cin >> N;
        vector<int> prereq(N), depend(N,0);
        REP(n,N) {
            int x; cin >> x; --x;
            prereq[n] = x;
            if (x >= 0) ++depend[x];
        }
        vector<int> subtreesize(N,1);
        vector<double> subtreeweight(N,1.);
        REP(kolo,N) REP(kto,N) { 
            subtreesize[kto]=1; 
            subtreeweight[kto]=1.;
            REP(druhy,N) if (prereq[druhy]==kto) {
                subtreesize[kto] += subtreesize[druhy];  
                subtreeweight[kto] *= subtreeweight[druhy];
            }
            subtreeweight[kto] *= subtreesize[kto];
        }

        string names;
        cin >> names;
        int M;
        cin >> M;
        vector<string> cool(M);
        REP(m,M) cin >> cool[m];

        STARTTIMER;
        int total = 0;
        vector<int> counts(M,0);
        while (true) {
            REP(kolo,100) simulate(prereq,depend,subtreesize,subtreeweight,names,cool,counts);
            total += 100;
            if (GETTIMER > 1000000) break;
        }
        printf("Case #%d:",test);
        REP(m,M) printf(" %.7f",(1.*counts[m])/total);
        printf("\n");
    }
}
// vim: fdm=marker:commentstring=\ \"\ %s:nowrap:autoread
