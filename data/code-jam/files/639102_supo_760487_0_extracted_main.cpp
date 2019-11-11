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
#define LIM 2000005
ll L,N;
ll B[105];
ll DP[LIM+1005];
void solve_case()
{
  cin>>L>>N;
  REP(i,N)
    cin>>B[i];
  sort(B,B+N);
  REP(i,LIM)DP[i]=INF+1;
  DP[0]=0;
  REP(i,LIM)
    REP(j,N)
      DP[i+B[j]]=min(DP[i+B[j]],DP[i]+1);  
  ll maxi=B[N-1];
  ll nas=L/maxi;
  ll res=1000000000000000001LL;
  while(nas>=0 &&  L - nas*maxi <LIM)
  {
    if (DP[L-nas*maxi]<INF)
      res=min(res,nas+DP[L-nas*maxi]);

    nas--;
  }
  if (1000000000000000001LL == res)
    puts("IMPOSSIBLE");
  else
  cout<<res<<endl;


    





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
