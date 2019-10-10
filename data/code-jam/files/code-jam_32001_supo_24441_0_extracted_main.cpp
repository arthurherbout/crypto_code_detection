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
#define INF 1000000
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
int N;
#define LIM 15
double p[3];
double lo[3],hi[3];
double lT[3],rT[3];
double val[3];
int X[LIM],Y[LIM],Z[LIM],P[LIM];
double ternarySearch(double left,double right,int h) 
{
    if (h==3)
    {
      double maxP=0;
      REP(i,N)
        maxP=max(maxP,(abs(X[i]-p[0])+abs(Y[i]-p[1])+abs(Z[i]-p[2]))/double(P[i]));
      return maxP;
    }
    double mini=INF;    
    REP(i,150)
    {
      double lT = (left*2+right)/3;
      double rT = (left+right*2)/3;
      p[h]=lT;
      double valL=ternarySearch(0,INF,h+1);
      p[h]=rT;
      double valR=ternarySearch(0,INF,h+1);
      mini=min(valL,mini);
      mini=min(valR,mini);      
      if (valL > valR)
        left=lT;
      else
        right=rT;
    }
    return mini;
}


double loX,loY,loZ,hiX,hiY,hiZ;
void solve_case()
{
  scanf("%d",&N);
  REP(i,N)
    scanf("%d %d %d %d",&X[i],&Y[i],&Z[i],&P[i]);
  printf("%.10lf\n",ternarySearch(0,INF,0));

}


int cases;
int main( )
{
  scanf("%d\n",&cases);
  REP(ii,cases)
  {
    printf("Case #%d: ",ii+1);
    solve_case();
  }         
  return 0;
}
