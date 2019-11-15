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
#define MOD 1000000009
int N,K;
vector<int> G[505];
int COLS;
ll memof[505];
inline ll mult(ll a,ll b,ll mod)
{
  ll x=0;a%=mod;
  for(;b;b>>=1)
  {
    if (b&1)
      x=(x+a)%mod;
    a=(a*2)%mod;    
  }
  return x;
}

ll FACT(int x)
{
  if (x==1) return 1;
  if (memof[x]!=-1) return memof[x];
  return memof[x]=(FACT(x-1)*x)%MOD;
}
ll vyber(int what,int from)
{//maximum 500 from K
  if (what>from) return 0;
  ll ans=1;
  REP(i,what)
    ans=mult(ans,from-i,MOD);//(ans*(from-i))%MOD;
  return ans;
}
bool bad=0;
ll solve(int x,int prev,int used1,int used2)
{
//   cout<<x+1<<" "<<prev+1<<" "<<used1<<" "<<used2<<endl;
  if (bad) return 0;
  ll res=1;
  int poc1=0,poc2=0;
  REPS(i,G[x])
    if (G[x][i]!=prev)
    {      
      ll next=solve(G[x][i],x,1,poc1+used1);
      ll poss=K-used1-used2-poc2;
      if (poss<=0)
      {
        bad=1;
        return 0;
      }
      
      ll curRes=mult(next,poss,MOD);
      res=mult(res,curRes,MOD);
      poc2+=sz(G[G[x][i]]);
      poc1++;
    }
  return res;
}
void solve_case()
{
  scanf("%d %d",&N,&K);
  REP(i,N) G[i].clear();
  int a,b;
  REP(i,N-1)
  {
    scanf("%d %d",&a,&b),a--,b--;
    G[a].pb(b);
    G[b].pb(a);    
  }
  bad=0;
  ll res=solve(0,-1,0,0);
  if (bad) cout<<0<<endl;
  else
  cout<<res%MOD<<endl;
}


int cases;
int main( )
{
  mset(memof,-1);
  scanf("%d",&cases);getchar();
  REP(ii,cases)
  {
    printf("Case #%d: ",ii+1);
    solve_case();
  }         
  return 0;
}
