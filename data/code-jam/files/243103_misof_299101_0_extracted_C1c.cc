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
#include <cstring>
#include <cctype>
#include <cassert>

#include <cmath>
#include <complex>
using namespace std;
// }}}

/////////////////// PRE-WRITTEN CODE FOLLOWS, LOOK DOWN FOR THE SOLUTION ////////////////////////////////

// pre-written code {{{
#define CLEAR(t) memset((t),0,sizeof(t))
#define FOR(i,a,b) for(int i=(int)(a);i<=(int)(b);++i)
#define FORD(i,a,b) for(int i=(int)(a);i>=(int)(b);--i)
#define REP(i,n) for(int i=0;i<(int)(n);++i)
// }}}

/////////////////// CODE WRITTEN DURING THE COMPETITION FOLLOWS ////////////////////////////////

int G[128][128];
vector< pair<int,int> > players;
int N;
int Y[128];
int reach[2][15000000];
bool present[16];

bool bipartite() {
  int color[128];
  REP(n,N) color[n] = -1;
  REP(n,N) if (color[n]==-1) {
    color[n] = 0;
    queue<int> Q;
    Q.push(n);
    while (!Q.empty()) {
      int x = Q.front();
      Q.pop();
      REP(y,N) if (G[x][y]) {
        if (color[y]==-1) {
          color[y] = !color[x];
          Q.push(y);
        } else {
          if (color[y]==color[x]) return false;
        }
      }
    }
  }
  return true;
}

int go[15000000];

bool tricolor() {
  CLEAR(reach[0]);
  reach[0][0] = 1;
  CLEAR(present);
  int last = 0, curr = 1;
  int upper = 1;
  REP(i,15) upper *= 3;

  FORD(q,N-1,0) {
    CLEAR(reach[curr]);
    // coloring footballer # q
    // try all previous colorings, for each of them try all ways to place him
    int y = Y[q];
    REP(ako,upper) {
      if (!reach[last][ako]) continue;

      int c[16], tmp=ako;
      // REP(i,15) { c[i]=tmp%3; tmp/=3; }
      REP(i,15) c[i] = (go[ako] >> (2*i)) & 3;

      bool ok = true;
      REP(i,15) if (!present[i]) if (c[i] != 0) ok = false;
      if (!ok) continue;

      REP(i,15) if (!present[i]) c[i] = 3;

      int moze[4];
      moze[0] = moze[1] = moze[2] = 1;
      if (y>0) moze[ c[y-1] ] = 0;
      moze[ c[y] ] = 0;
      if (y<14) moze[ c[y+1] ] = 0;
      for (int f=0; f<3; ++f) if (moze[f]) {
        c[y] = f;
        int kod = 0;
        FORD(i,14,0) { kod *= 3; kod += c[i]%3; }
        reach[curr][kod] = true; 
      }
    }
    swap(last,curr);
  }
  REP(ako,upper) if (reach[last][ako]) return true;
  return false;
}

int main() {
  int upper = 1;
  REP(i,15) upper *= 3;

  REP(ako,upper) {
    int c[16], tmp=ako;
    REP(i,15) { c[i]=tmp%3; tmp/=3; }
    int res = 0;
    FORD(i,14,0) { res <<= 2; res |= c[i]; }
    go[ako] = res;
  }

  int T;
  cin >> T;
  FOR(t,1,T) {
    cin >> N;
    players.clear();
    REP(n,N) { int x,y; cin >> x >> y; --y; players.push_back(make_pair(x,y)); }
    sort(players.begin(),players.end());
    REP(n,N) Y[n] = players[n].second;

    bool disjoint = true;
    REP(a,N) REP(b,a) if (abs(Y[a]-Y[b])<=1) disjoint = false;
    if (disjoint) {
      printf("Case #%d: 1\n",t);
      continue;
    }

    CLEAR(G);
    REP(n,N) {
      for (int d=-1; d<=1; ++d) {
        for (int x=n+1; x<N; ++x) {
          if (Y[x] == Y[n] + d) {
            G[x][n] = G[n][x] = 1;
            break;
          }
        }
      }
    }
    if (bipartite()) {
      printf("Case #%d: 2\n",t);
      continue;
    }
    /*
    if (tricolor()) {
      printf("Case #%d: 3\n",t);
      continue;
    }
    */
    printf("Case #%d: 3\n",t);
  }
  return 0;
}
// vim: fdm=marker:commentstring=\ \"\ %s:nowrap:autoread
