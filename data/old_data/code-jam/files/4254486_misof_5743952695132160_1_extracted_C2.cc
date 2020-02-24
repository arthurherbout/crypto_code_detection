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
// BEGIN CUT HERE
#define DEBUG(var) { cout << #var << ": " << (var) << endl; }
template <class T> ostream& operator << (ostream &os, const vector<T> &temp) { os << "[ "; for (unsigned int i=0; i<temp.size(); ++i) os << (i?", ":"") << temp[i]; os << " ]"; return os; } // DEBUG
template <class T> ostream& operator << (ostream &os, const set<T> &temp) { os << "{ "; for(__typeof((temp).begin()) it=(temp).begin();it!=(temp).end();++it) os << ((it==(temp).begin())?"":", ") << (*it); os << " }"; return os; } // DEBUG
template <class T> ostream& operator << (ostream &os, const multiset<T> &temp) { os << "{ "; for(__typeof((temp).begin()) it=(temp).begin();it!=(temp).end();++it) os << ((it==(temp).begin())?"":", ") << (*it); os << " }"; return os; } // DEBUG
template <class S, class T> ostream& operator << (ostream &os, const pair<S,T> &a) { os << "(" << a.first << "," << a.second << ")"; return os; } // DEBUG
template <class S, class T> ostream& operator << (ostream &os, const map<S,T> &temp) { os << "{ "; for(__typeof((temp).begin()) it=(temp).begin();it!=(temp).end();++it) os << ((it==(temp).begin())?"":", ") << (it->first) << "->" << it->second; os << " }"; return os; } // DEBUG
namespace aux{
    template<std::size_t...> struct seq{};
    template<std::size_t N, std::size_t... Is> struct gen_seq : gen_seq<N-1, N-1, Is...>{};
    template<std::size_t... Is> struct gen_seq<0, Is...> : seq<Is...>{};
    template<class Ch, class Tr, class Tuple, std::size_t... Is> void print_tuple(std::basic_ostream<Ch,Tr>& os, Tuple const& t, seq<Is...>) { using swallow = int[]; (void)swallow{0, (void(os << (Is == 0? "" : ", ") << std::get<Is>(t)), 0)...}; }
} // aux::
template<class Ch, class Tr, class... Args> auto operator<<(std::basic_ostream<Ch, Tr>& os, std::tuple<Args...> const& t) -> std::basic_ostream<Ch, Tr>& { os << "("; aux::print_tuple(os, t, aux::gen_seq<sizeof...(Args)>()); return os << ")"; }
// END CUT HERE
#define FOR(i,a,b) for(int i=(int)(a);i<=(int)(b);++i)
#define REP(i,n) for(int i=0;i<(int)(n);++i)
#define SIZE(t) ((int)((t).size()))
// }}}

/////////////////// CODE WRITTEN DURING THE COMPETITION FOLLOWS ////////////////////////////////

set<long long> kands;

map<long long,long long> reduce(map<long long,long long> pocty, long long prvok) {
    map<long long,long long> EMPTY;

    if (prvok < 0) {
        for (map<long long,long long>::reverse_iterator it = pocty.rbegin(); it != pocty.rend(); ++it) {
            if (it->second == 0) continue;
            long long vacsi = it->first + prvok;
            if (!pocty.count(vacsi)) return EMPTY;
            if (pocty[vacsi] < it->second) return EMPTY;
            pocty[vacsi] -= it->second;
            if (pocty[vacsi] == 0) pocty.erase(vacsi);
        }
    }

    if (prvok == 0) {
        for (map<long long,long long>::iterator it = pocty.begin(); it != pocty.end(); ++it) {
            if (it->second % 2) return EMPTY;
            it->second /= 2;
        }
    }

    if (prvok > 0) {
        for (map<long long,long long>::iterator it = pocty.begin(); it != pocty.end(); ++it) {
            if (it->second == 0) continue;
            long long vacsi = it->first + prvok;
            if (!pocty.count(vacsi)) return EMPTY;
            if (pocty[vacsi] < it->second) return EMPTY;
            pocty[vacsi] -= it->second;
            if (pocty[vacsi] == 0) pocty.erase(vacsi);
        }
    }
    
    return pocty;
}

vector<long long> solve(map<long long,long long> pocty, int steps) {
    /*
    DEBUG("solve");
    DEBUG(pocty);
    DEBUG(steps);
    */
    if (steps == 1) {
        if (pocty[0] == 2) return vector<long long>(1,0);
        if (pocty[0] == 0) return vector<long long>();
        for (auto item:pocty) if (item.first != 0) return vector<long long>(1,item.first);
    } else {
        vector<long long> kand;
        long long mins = pocty.begin()->first;
        if (mins < 0) {
            // min je zaporny, hladame len zapornych kandidatov
            for (auto item:pocty) if (item.first != mins) if (item.first <= 0) if (kands.count(mins - item.first)) kand.push_back( mins - item.first );
        } else {
            // min je prazdny set, hladame nezapornych kandidatov
            assert(mins == 0);
            if (pocty.begin()->second > 1) kand.push_back(0);
            if (kand.empty()) {
                for (auto item:pocty) if (item.first != mins) {
                    kand.push_back(item.first);
                    break;
                }
            }
        }
        sort( kand.begin(), kand.end() );
        for (auto k : kand) {
            if (!kands.count(k)) continue;
            map<long long, long long> npocty = reduce(pocty,k);
            //if (npocty.empty()) cout << "kand " << k << " failed 1" << endl;
            if (npocty.empty()) continue;
            vector<long long> nsolve = solve(npocty,steps-1);
            //if (nsolve.empty()) cout << "kand " << k << " failed 2" << endl;
            if (nsolve.empty()) continue;
            nsolve.push_back(k);
            return nsolve;
        }
    }
    return vector<long long>();
}

set<long long> get_kands(const vector<long long> &vals, const vector<long long> &muls, int steps) {
    int P = SIZE(vals);
    map<long long,long long> pocty;
    REP(p,P) pocty[ vals[p] ] = muls[p];
    
    set<long long> kands;
    for (int i=1; i<SIZE(vals); ++i) {
        long long kand = vals[i] - vals[0];
        long long pocet = 0;
        REP(p,P) { 
            if (!pocty.count(vals[p]+kand)) continue;
            pocet += min( muls[p], pocty[vals[p]+kand] );
        }
        if (pocet >= (1LL << (steps-1))) kands.insert(kand);
    }
    for (int i=1; i<SIZE(vals); ++i) {
        long long kand = vals[0] - vals[i];
        long long pocet = 0;
        REP(p,P) { 
            if (!pocty.count(vals[p]+kand)) continue;
            pocet += min( muls[p], pocty[vals[p]+kand] );
        }
        if (pocet >= (1LL << (steps-1))) kands.insert(kand);
    }
    kands.insert(0);
    return kands;
}

int main(int argc, char**argv) {
    int T; cin >> T;
    int LO; stringstream(argv[1]) >> LO;
    int HI; stringstream(argv[2]) >> HI;
    FOR(t,1,T) {
        int P; cin >> P;
        vector<long long> vals(P), muls(P);
        REP(p,P) cin >> vals[p];
        REP(p,P) cin >> muls[p];

        if (t < LO || t > HI) continue;
        
        long long suma = accumulate( muls.begin(), muls.end(), 0LL );
        int steps = 0;
        long long ssuma = 1;
        while (ssuma != suma) ++steps, ssuma*=2;

        kands = get_kands(vals,muls,steps);

        map<long long,long long> pocty;
        REP(p,P) pocty[ vals[p] ] = muls[p];
        vector<long long> answer = solve(pocty,steps);
        sort( answer.begin(), answer.end() );
        cout << "Case #" << t << ":";
        for (long long x:answer) cout << " " << x;
        cout << endl;
    }
}
// vim: fdm=marker:commentstring=\ \"\ %s:nowrap:autoread
