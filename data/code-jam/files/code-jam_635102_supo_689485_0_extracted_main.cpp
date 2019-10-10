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

int N,M;
pii P[1005];
pii Q[1005];
double getDist(pii a,pii b)
{
  return sqrt( SQR(a.fi-b.fi) + SQR(a.se-b.se));
}
double solve(pii A,double R0,pii B,double R1)
{
  double C=getDist(A,B);
  double areaA=PI*R0*R0;
  double areaB=PI*R1*R1;
  if (C>=R0+R1)
    return 0;
  if (C<abs(R0-R1))
    return min(areaA,areaB);
  
  double CBA=acos((R1*R1 + C*C - R0*R0)/double(2*R1*C));
  double CAB=acos((R0*R0 + C*C - R1*R1)/double(2*R0*C));
  double CAD=2*CAB;
  double CBD=2*CBA;
  double inter= 0.5*CBD*R1*R1 - 0.5*R1*R1*sin(CBD) + 0.5*CAD*R0*R0 - 0.5*R0*R0*sin(CAD);
  return inter;
}

void solve_case()
{
  scanf("%d %d",&N,&M);
  REP(i,N)
    scanf("%d %d",&P[i].fi, &P[i].se);

  REP(i,M)
    scanf("%d %d",&Q[i].fi, &Q[i].se);

  //kruznice so stredmi na P a polomerom D(P_i,Q_k)

  REP(k,M)
  {
    if (k)putchar(' ');
    printf("%.8lf",solve(P[0],getDist(P[0],Q[k]),P[1],getDist(P[1],Q[k])));
  }
  putchar(10);
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
