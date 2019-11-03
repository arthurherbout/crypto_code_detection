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

#define OFF 2000000
int C;

int T[OFF*2+10];

set<pii> Q;
void solve_case()
{
  scanf("%d",&C);
  mset(T,0);
  Q.clear();
  REP(i,C)
  {
    int a,b;
    scanf("%d %d",&a,&b);a+=OFF;
    T[a]=b; 
    if (b>1)
    Q.insert(mp(-b,a)); 
  }
  int step=0;
  while(sz(Q))
  {
    pii top=*Q.begin();
    FOR(i,-1,1)
      Q.erase(mp(-T[top.se+i],top.se+i));
    T[top.se-1]++;
    T[top.se+1]++;
    T[top.se]-=2;
    FOR(i,-1,1)
      if (T[top.se+i]>1)
        Q.insert(mp(-T[top.se+i],top.se+i));
    step++;
  }
  cout<<step<<endl;





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
