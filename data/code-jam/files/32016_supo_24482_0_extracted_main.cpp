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
using namespace std;
int cases,a,b,N,M,T;
int P[2005];
vector<pii> V[2005];
int onePos[2005];
pii zak[2005];
int main( )
{
  scanf("%d",&cases);
  REP(ii,cases)
  {
    scanf("%d %d",&N,&M);
    mset(P,0);
    mset(onePos,-1);
    REP(i,M)
    {
      scanf("%d",&T);
      zak[i]=mp(T,i);
      V[i].clear();
      REP(j,T)
      {
        scanf("%d %d",&a,&b);
        a--;
        if (b==1) onePos[i]=a;
        V[i].pb(mp(a,b));
      }
    }
    sort(zak,zak+M);
    REP(kde,M)
    {
      int i=zak[kde].se;
      int isOk=0;
      FORE(it,V[i])
        if (P[it->fi]==it->se)
        {
          isOk=1;
          break;
        }
      if (!isOk&&onePos[i]!=-1)      
        P[onePos[i]]=1;      
    }
    int isTotOk=1;
    REP(i,M)
    {
      int isOk=0;
      FORE(it,V[i])
        if (P[it->fi]==it->se)
        {
          isOk=1;
          break;
        }
      if (!isOk)
      {
        isTotOk=0;
        break;
      }
    }
    printf("Case #%d: ",ii+1);
    if (isTotOk)
    {
      REP(i,N)
        printf("%d%c",P[i],i==N-1?'\n':' ');
    }else
      printf("IMPOSSIBLE\n");
  }

  return 0;
}
