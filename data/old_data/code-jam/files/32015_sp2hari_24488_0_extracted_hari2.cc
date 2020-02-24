#include <vector>
#include <algorithm>
#include <string>
#include <iostream>
#include <cmath>
#include <set>
#include <queue>
using namespace std;
#define GI ({LL t;scanf("%Ld",&t);t;})
#define LET(x,a) __typeof(a) x(a)
#define FOR(i,a,b) for(LET(i,a);i<(b);++i)
#define EACH(x,v) for(typeof(v.begin()) x=v.begin();x<v.end();x++)
#define REP(i,n) FOR(i,0,n)
#define pb push_back
#define mkp make_pair
#define sz size()
#define bset(i,j) (i&(1<<j))
#define INF (int)1e8
#define MAX 402
#define alog(x) exp(x*log(10.0))
typedef long long LL;
typedef double D;
typedef vector<int> VI;
using namespace std;
int main()
{
 #define M 1000000007    
  int t=GI, kase=1;
  while(t--) {
     printf("Case #%d: ", kase++);
     LL n=GI, m=GI, a[m], b[n], x=GI, y=GI, z=GI, dp[n];
     LL ans=0;
     memset(dp,0,sizeof(dp));
     REP(i,m) {
          a[i]=GI;
         }
     REP(i,n) {
             b[i]=a[i%m];
         a[i%m]=(x*a[i%m]+y*(i+1))%z;
     }
     REP(i,n) {
          dp[i]=1;
          REP(j,i) if(b[i] > b[j]) {
               dp[i] = (dp[i]+dp[j])%M;
          }
          ans=(ans+dp[i])%M;
     }
     cout<<ans<<"\n";
  }
  
}
