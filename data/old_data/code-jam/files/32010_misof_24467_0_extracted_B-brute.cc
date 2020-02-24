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

#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cassert>
#include <cstring>

#include <cmath>
#include <complex>
using namespace std;
// }}}

/////////////////// PRE-WRITTEN CODE FOLLOWS, LOOK DOWN FOR THE SOLUTION ////////////////////////////////

// pre-written code {{{
#define REP(i,n) for(int i=0;i<(int)(n);++i)
// }}}

/////////////////// CODE WRITTEN DURING THE COMPETITION FOLLOWS ////////////////////////////////

int T; string Tline;

int F[500],LO[500],HI[500];
int C, N;

class interval {
  public: 
    int lo, hi;
    interval(int lo, int hi) : lo(lo), hi(hi) { }
};

bool operator< (const interval &A, const interval &B) {
  if (A.hi != B.hi) return A.hi > B.hi;
  return false;
}

bool firstcmp (const interval &A, const interval &B) {
  if (A.lo != B.lo) return A.lo < B.lo;
  return false;
}


int doit(int x, int y, int z) {
  vector<interval> cand;
  for (int i=0; i<N; i++) if (F[i]==x || F[i]==y || F[i]==z) cand.push_back( interval( LO[i], HI[i] ) );
  cand.push_back(interval(10047,10047));
  sort( cand.begin(), cand.end(), firstcmp );
  priority_queue<interval> Q;
  int kde = 1, k = 0;
  int used = 0;
  while (true) {
    while (cand[k].lo <= kde) Q.push( cand[k++] );
    if (Q.empty()) return 999;
    interval pouzi = Q.top(); Q.pop();
    if (pouzi.hi <= kde) return 999;
    used++;
    kde = pouzi.hi + 1;
    if (kde == 10001) break;
  }
  return used;
}

int main() {
  getline(cin,Tline); stringstream(Tline) >> T;
  for (int t=1; t<=T; t++) {
    cin >> N;
    map<string,int> color;
    C=0;
    string ccc;
    REP(i,N) { cin >> ccc >> LO[i] >> HI[i]; if (!color.count(ccc)) color[ccc]=C++; F[i]=color[ccc]; }

    int res = 999;

    REP(take,1<<N) {
      set<int> colorz;
      REP(i,N) if (take & (1<<i)) colorz.insert(F[i]);
      if (colorz.size() > 3) continue;
      int colored[10002];
      memset(colored,0,sizeof(colored));
      REP(i,N) if (take & (1<<i)) for (int j=LO[i]; j<=HI[i]; j++) colored[j]=1;
      int sum=0;
      for (int i=1; i<=10000; i++) sum += colored[i];
      if (sum == 10000) res = min(res, __builtin_popcount(take) );
    }

    // for (int i=0; i<C; i++) for (int j=0; j<=i; j++) for (int k=0; k<=j; k++) res = min(res, doit(i,j,k) );
    if (res == 999) {
      printf("Case #%d: IMPOSSIBLE\n",t);
    } else {
      printf("Case #%d: %d\n",t,res);
    }
  }
  return 0;
}
// vim: fdm=marker:commentstring=\ \"\ %s:nowrap:autoread
