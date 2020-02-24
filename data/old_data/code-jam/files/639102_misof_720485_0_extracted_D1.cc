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
#define SIZE(t) ((int)((t).size()))
// }}}

/////////////////// CODE WRITTEN DURING THE COMPETITION FOLLOWS ////////////////////////////////

int N, B;
int MOD = 1000000007;

class state {
  public:
  int remains, last;
  vector<int> forbidden;
};
bool operator< (const state &A, const state &B) {
  if (A.remains != B.remains) return A.remains < B.remains;
  if (A.last != B.last) return A.last < B.last;
  return A.forbidden < B.forbidden;
}

map<state,int> memo;

int solve(int remains, int last, vector<int> forbidden) {
  state S;
  S.remains = remains; S.last = last; S.forbidden = forbidden;
  if (memo.count(S)) return memo[S];
  int &res = memo[S];
  res = 0;
  if (remains==0) return res = 1;

  for (int cur = 1; cur <= last; ++cur) {
    vector<int> digits;
    int tmp = cur;
    while (tmp) { digits.push_back( tmp % B ); tmp /= B; }
    if (SIZE(forbidden) < SIZE(digits)) forbidden.resize( SIZE(digits), 0 );
    bool ok = true;
    REP(i,SIZE(digits)) if (forbidden[i] & 1 << digits[i]) { ok = false; break; }
    if (ok) {
      vector<int> nforb = forbidden;
      REP(i,SIZE(digits)) nforb[i] |= 1 << digits[i];
      res += solve(remains-cur, cur, nforb);
      res %= MOD;
    }
  }
  return res;
}

int main() {
  int T;
  cin >> T;
  FOR(t,1,T) {
    cin >> N >> B;
    memo.clear();
    cout << "Case #" << t << ": " << solve(N,N,vector<int>()) << endl;
  }
  return 0;
}
// vim: fdm=marker:commentstring=\ \"\ %s:nowrap:autoread
