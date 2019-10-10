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

int M,N;
int MAT[15];
char ch;
int DP[1<<12][13];
int solve(int m,int r)
{
  int &res=DP[m][r];
  if (res!=-1) return res;
  if (r==M) return res=0;
  int H=1<<N;
  bool isbad[15];
  res=0;
  REP(mask,H)
  if ((m&mask)==mask&&(MAT[r]&mask)==mask)
  {
    //dve pri sebe
    int cur=mask,okay=1;
    while(cur)
    {
      if ((cur&3)==3)okay=0;
      cur/=2;
    }
    if (!okay)continue;

    mset(isbad,0);    
    REP(i,N)
      if (mask&(1<<i))
      {
        if (i-1>=0)isbad[i-1]=1;
        if (i+1<N)isbad[i+1]=1;
      }
    int newm=0;
    REP(i,N)
      if (!isbad[i])newm|=1<<i;

    res=max(res,solve(newm,r+1)+__builtin_popcount(mask));      
  }
  return res;
}
void solve_case()
{
  scanf("%d %d",&M,&N);
  mset(MAT,0);
  REP(i,M)
    REP(j,N)
    {
      cin>>ch;
      if (ch=='.')MAT[i]|=1<<j;
    }
  mset(DP,-1);
  printf("%d\n",solve((1<<N)-1,0));  
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
