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
#define FOREACH(it,c) for(__typeof((c).begin()) it=(c).begin();it!=(c).end();++it)
// }}}

/////////////////// CODE WRITTEN DURING THE COMPETITION FOLLOWS ////////////////////////////////

char images[26][3] = {
"a4",
"b8",
"c",
"d",
"e3",
"f",
"g9",
"h",
"i1",
"j",
"k",
"l",
"m",
"n",
"o0",
"p",
"q",
"r",
"s5",
"t7",
"u",
"v",
"w",
"x",
"y",
"z" };

int main() {
    int T; cin >> T;
    FOR(t,1,T) {
        int K; cin >> K;
        string S; cin >> S;
        int N = S.size();
        set<string> edges;
        for (int n=0; n+1<N; ++n) {
            int a = S[n]-'a', b = S[n+1]-'a';
            for (int i=0; i<strlen(images[a]); ++i) {
                for (int j=0; j<strlen(images[b]); ++j) {
                    string tmp = "";
                    tmp += images[a][i];
                    tmp += images[b][j];
                    edges.insert(tmp);
                }
            }
        }
        long long edge_count = 0;
        map<char,int> deg;
        FOREACH(it,edges) {
            ++deg[(*it)[0]]; 
            --deg[(*it)[1]]; 
            ++edge_count;
        }
        int add = 0;
        FOREACH(it,deg) if (it->second > 0) add += it->second;
        if (add == 0) {
            cout << "Case #" << t << ": " << (edge_count+1) << endl;
        } else {
            cout << "Case #" << t << ": " << (edge_count+add) << endl;
        }
    }
  return 0;
}
// vim: fdm=marker:commentstring=\ \"\ %s:nowrap:autoread
