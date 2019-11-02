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
#define LIM 1000005
int D,K;
int S[15];
bool isP[LIM];
void solve_case()
{
  scanf("%d %d",&D,&K);

  REP(i,K)
    scanf("%d",&S[i]);
if (K==1)
{
  puts("I don't know.");
  return;
}  
  int hran=1;
  REP(i,D)hran*=10;

  set<int> Q;
  FOR(P,2,hran)
    if (isP[P])
    {
      REP(zvA,P)
        for (int zvB=0;zvB<P;)
        if (S[1] == (zvA*S[0] + zvB)%P)
        {
          bool isok=1;
          FOR(i,0,K-2)
            if (S[i+1]!= (zvA*S[i]+zvB)%P)
              isok=0;
          if (!isok){zvB++;continue;}

          int ns=(zvA*S[K-1]+zvB)%P;
          Q.insert(ns);
          if (sz(Q)>1)goto hell;
          zvB++;

        }else
        {
  
          int cur=(zvA*S[0] + zvB)%P;
          if (cur<S[1])zvB+=S[1]-cur;
          else zvB=P;
        }    
    }
hell:;
     if (sz(Q)>1 || sz(Q)==0)
       puts("I don't know.");
     else
       printf("%d\n",*Q.begin());






}


int cases;
int main( )
{
  REP(i,LIM)
    isP[i]=1;
  for (int i=2;i<LIM;i++)
    if (isP[i])
      for (int j=2*i;j<LIM;j+=i)
        isP[j]=0;

  scanf("%d",&cases);getchar();
  REP(ii,cases)
  {
    printf("Case #%d: ",ii+1);
    solve_case();
  }         
  return 0;
}
