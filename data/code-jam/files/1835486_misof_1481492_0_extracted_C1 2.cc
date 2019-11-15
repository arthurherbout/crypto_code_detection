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
// BEGIN CUT HERE
#define DEBUG(var) { cout << #var << ": " << (var) << endl; }
template <class T> ostream& operator << (ostream &os, const vector<T> &temp) { os << "[ "; for (unsigned int i=0; i<temp.size(); ++i) os << (i?", ":"") << temp[i]; os << " ]"; return os; } // DEBUG
template <class T> ostream& operator << (ostream &os, const set<T> &temp) { os << "{ "; for(__typeof((temp).begin()) it=(temp).begin();it!=(temp).end();++it) os << ((it==(temp).begin())?"":", ") << (*it); os << " }"; return os; } // DEBUG
template <class T> ostream& operator << (ostream &os, const multiset<T> &temp) { os << "{ "; for(__typeof((temp).begin()) it=(temp).begin();it!=(temp).end();++it) os << ((it==(temp).begin())?"":", ") << (*it); os << " }"; return os; } // DEBUG
template <class S, class T> ostream& operator << (ostream &os, const pair<S,T> &a) { os << "(" << a.first << "," << a.second << ")"; return os; } // DEBUG
template <class S, class T> ostream& operator << (ostream &os, const map<S,T> &temp) { os << "{ "; for(__typeof((temp).begin()) it=(temp).begin();it!=(temp).end();++it) os << ((it==(temp).begin())?"":", ") << (it->first) << "->" << it->second; os << " }"; return os; } // DEBUG
// END CUT HERE
#define FOR(i,a,b) for(int i=(int)(a);i<=(int)(b);++i)
#define REP(i,n) for(int i=0;i<(int)(n);++i)
// }}}

/////////////////// CODE WRITTEN DURING THE COMPETITION FOLLOWS ////////////////////////////////

long long M, F; int N;
vector<long long> P, S, bestpack, memo;

long long solve(long long money) {
    assert( money >= 0 );
    if (money < bestpack[1]) return 0;
    if (memo[money] >= 0) return memo[money];
    long long &res = memo[money];
    res = 0;
    // binarne vyhladame kolko najviac mozeme nakupit
    long long lo=1, hi=M+1;
    while (hi-lo > 1) {
        long long med = lo+(hi-lo)/2;
        if (bestpack[med]<=money) lo=med; else hi=med;
    }
    res = max( res, lo+solve(money-bestpack[lo]) );
    // skusime pouzit vsetky opti speci baliky
    REP(n,N) {
        long long cena = bestpack[S[n]+1];
        if (cena > money) continue;
        res = max( res, S[n]+1+solve(money-cena) );
    }
    // skusime vsetky male
    FOR(n,1,11) if (bestpack[n]<=money) res = max( res, n+solve(money-bestpack[n]) );
    FOR(n,1,11) {
        int x = 1 + rand()%lo;
        if (bestpack[x]<=money) res = max( res, x+solve(money-bestpack[x]) );
    }
    return res;
}

int main() {
    int T; cin >> T;
    FOR(t,1,T) {
        cin >> M >> F >> N;
        cerr << M << " " << F << " " << N << endl;
        P.clear(); P.resize(N);
        S.clear(); S.resize(N);
        REP(n,N) { cin >> P[n] >> S[n]; }
        bestpack.clear(); bestpack.resize(2000001,M+1);
        bestpack[0] = F;
        REP(n,N) {
            // vec n mozeme kupit nanajvys na dni 0..S[n]
            // DEBUG(n);
            // DEBUG(S[n]);
            for (int d=0; d<=S[n]; ++d) bestpack[d+1] = min( bestpack[d+1], bestpack[d] + P[n] );
        }
        memo.clear(); memo.resize(2000001,-1);
        cout << "Case #" << t << ": " << solve(M) << endl;
    }
}
// vim: fdm=marker:commentstring=\ \"\ %s:nowrap:autoread
