#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cmath>
#include <list>
#include <cstring>
#include <stack>
#include <bitset>

using namespace std;

#define NMAX 2147483647
#define LMAX 9223372036854775807LL
#define pb push_back
#define pob pop_back
#define mp make_pair
#define st first
#define nd second
#define FOR(i,a,b) for(int i=(a);i<(b);++i)
#define FORD(i,a,b) for(int i=(a);i>(b);--i)
#define REP(i,n) FOR(i,0,n)
#define FORE(i,a,b) for(int i=(a);i<=(b);++i)

int N, L, ans;
vector <string> L1, L2;
vector <string> g6;
bool change[155];

int solve() {
  sort(L2.begin(), L2.end());
  int ans = 1000;
  REP(i, N) {
    memset(change,0,sizeof(change));
    int temp = 0;
    g6 = L1;
    REP(j,L) {
      if(L1[0][j] != L2[i][j])
        change[j] = 1, temp++;;
    }
    REP(j,N) REP(k,L)
      if(change[k]) g6[j][k] = (g6[j][k] == '1' ? '0' : '1');
    sort(g6.begin(), g6.end());
    if(g6 == L2) {
      ans = min(ans, temp);
    }
  }
  if(ans == 1000)
    return -1;
  return ans;
}


int main()
{
  freopen("input.txt","r",stdin);
  freopen("out.txt","w",stdout);
  int TS;
  scanf("%d",&TS);
  FORE(ts,1,TS) {
    L1.clear();
    L2.clear();
    string temp;
    cin >> N >> L;
    REP(i,N) cin >> temp, L1.pb(temp);
    REP(i,N) cin >> temp, L2.pb(temp);
    int ans = solve();
    if(ans != -1)
    printf("Case #%d: %d\n",ts, solve());
    else printf("Case #%d: NOT POSSIBLE\n",ts);
  }
  return 0;
}
