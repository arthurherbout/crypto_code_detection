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

int main() {
    int T; cin >> T;
    FOR(test,1,T) {
        int N; cin >> N;
        vector<string> action(N);
        vector<int> ID(N);
        REP(n,N) cin >> action[n] >> ID[n];

        set<int> present;
        REP(n,N) if (ID[n]>0) present.insert(ID[n]);
        map<int,int> relabel;
        int lab = 1;
        for (auto x:present) relabel[x] = lab++;
        REP(n,N) if (ID[n]>0) ID[n] = relabel[ID[n]];

        set<int> valid_states;
        REP(n,1<<N) valid_states.insert(n);

        REP(n,N) {
            set<int> new_valid_states;
            for (int state:valid_states) {
                if (action[n][0]=='E') {
                    if (ID[n]==0) {
                        REP(i,N) if (!(state & 1<<i)) new_valid_states.insert(state | 1<<i);
                    } else {
                        int x = ID[n]-1;
                        if (!(state & 1<<x)) new_valid_states.insert(state | 1<<x);
                    }
                } else {
                    if (ID[n]==0) {
                        REP(i,N) if ((state & 1<<i)) new_valid_states.insert(state ^ 1<<i);
                    } else {
                        int x = ID[n]-1;
                        if ((state & 1<<x)) new_valid_states.insert(state ^ 1<<x);
                    }
                }
            }
            valid_states = new_valid_states;
        }
        if (valid_states.empty()) {
            cout << "Case #" << test << ": CRIME TIME" << endl;
        } else {
            int best = 15;
            for (int x:valid_states) best = min( best, __builtin_popcount(x));
            cout << "Case #" << test << ": " << best << endl;
        }
    }
  return 0;
}
// vim: fdm=marker:commentstring=\ \"\ %s:nowrap:autoread
