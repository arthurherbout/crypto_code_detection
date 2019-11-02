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

vector <int> L[1111];
int N;
int pil;

pair <int, int> dfs(int x, int p) {
 // if(pil == 1)
  vector <pair <int, int> > s;
  int hit = 1;
  REP(i, L[x].size()) {
    if(L[x][i] == p) continue;
    s.pb(dfs(L[x][i], x));
    //if(x == 1) cout << L[x][i] << endl;
  }
  REP(i,s.size()) hit += s[i].nd;
  if(s.size() == 0)
    return mp(0, hit);
  if(s.size() == 1) {
    return mp(hit-1, hit);
  }
  int ans = NMAX;
  REP(i,s.size()) FOR(j,i+1,s.size()) {
    int temp = 0;
    REP(k,s.size())
      if(k == i || k == j)
        temp += s[k].st;
      else
        temp += s[k].nd;
    ans = min(temp, ans);
  }
  return mp(ans, hit);
}

int main()
{
  freopen("input.txt","r",stdin);
  freopen("out.txt","w",stdout);
  int TS;
  scanf("%d",&TS);
  FORE(ts,1,TS) {
    REP(i,1100) L[i].clear();
    scanf("%d",&N);
    int a , b;
    REP(i, N - 1) {
      scanf("%d%d",&a,&b);
      a--;b--;
      L[a].pb(b);
      L[b].pb(a);
    }
    int ans = NMAX;
    REP(i, N) {
      pil = i+1;
      pair <int, int> temp = dfs(i, -1);
      //cout << i+1 << " " << temp.st << endl;
      ans = min(ans, temp.st);
    }
    printf("Case #%d: %d\n",ts,ans);
  }
  return 0;
}
