//DEDICATED TO EMMA WATSON, THE BRITISH *SUNSHINE*
#include <vector>
#include <list>
#include <map>
#include <set>
#include <deque>
#include <stack>
#include <bitset>
#include <algorithm>
#include <functional>
#include <numeric>
#include <utility>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>
//#include <fstream>

#define eps 10e-10
#define INF 1000000000
#define PI 3.141592653589793238462
#define EU 2.71828182845904523536
#define sz(a) (int)a.size()
#define pb(a) push_back(a)
#define mset(a,hodnota) memset(a,hodnota,sizeof(a))
#define wh(a) a.begin(),a.end()
#define REP(i,n) for(__typeof(n) i=0;i<(n);++i)
#define REPS(i,n) for(int(i)=0;i<int(n.size());++i)
#define FOR(i,a,b) for(__typeof(b) i=(a);i<=(b);++i)
#define FORD(i,a,b) for(__typeof(a) i=(a);i>=(b);--i)
#define FORE(it,c) for(__typeof((c).begin()) it=(c).begin();it!=(c).end();++it)

#define SQR(a) ((a)*(a))
#define pii pair<int,int>
#define mp(a,b) make_pair(a,b)
#define fi first
#define se second
typedef long long ll;
typedef long double ld;
using namespace std;

pii seg[305];
string col[305];
set<string> COLS;
int DP[10005];
set<string> GOOD;
vector<pii> CUR;
int N;
int solve(int x)
{
  int &ret=DP[x];
  if (ret!=-1) return ret;
	if (x>10000) return 0;
  ret=INF;
  REPS(i,CUR)
    if (CUR[i].fi<=x)
      ret=min(ret,solve(CUR[i].se+1)+1);    
  return ret;
}
void solve_case()
{
  scanf("%d",&N);
  COLS.clear();
  REP(i,N)
  {
    cin>>col[i]>>seg[i].fi>>seg[i].se;
    COLS.insert(col[i]);
  }
  int ans=INF;
  vector<string> V;
	FORE(it,COLS)V.pb(*it);
  REPS(i,V)
    REPS(j,V)
    REPS(k,V)
      {
        GOOD.clear();
        GOOD.insert(V[i]);
        GOOD.insert(V[j]);
        GOOD.insert(V[k]);
        CUR.clear();
        REP(ii,N)
          if (GOOD.find(col[ii])!=GOOD.end())
            CUR.pb(seg[ii]);
				
        mset(DP,-1);        
        ans=min(ans,solve(1));
      }
  if (ans!=INF) cout<<ans<<endl;
  else cout<<"IMPOSSIBLE"<<endl;
}


int cases;
int main( )
{
  scanf("%d",&cases);getchar();
  REP(ii,cases)
  {
    printf("Case #%d: ",ii+1);
    solve_case();
  }         
  return 0;
}
