/*
    Rezwan_4029
    AUST , CSE-25
*/

#include<stdio.h>
#include<iostream>
#include<cstring>
#include<string>
#include<cctype>
#include<vector>
#include<algorithm>
#include<cmath>
#include<map>
#include<sstream>
#include<set>
#include<queue>
#define pb push_back
#define INF 1<<30
#define ms(a,v) memset(a,v,sizeof a)
#define II ({int a; scanf("%d", &a); a;})
#define LL ({ll  a; scanf("%lld", &a); a;})
#define FRD freopen("in.txt", "r", stdin)
#define FWR freopen("out.txt", "w", stdout)
using namespace std;
typedef long long ll;

#define rep(i,n) for( __typeof(n)i = 0 ; i < n ; i++)
#define For(i,n) for( __typeof(n)i = 1 ; i <= n ; i++)
#define forstl(i,n) for(__typeof(n.begin())i = n.begin();i!=n.end();i++)
#define forab(i,a,b)for(__typeof(b)i = a ; i <= (b) ; i++)

int main()
{
 // FRD; FWR;
  int test = II ;
  For(cs,test){
      ll r = LL , p = LL ,fp = 0 , ring = 0;
      while(p >= fp){
            fp += (2*r + 1 );
            if( fp <= p ) ring++;
            r += 2 ;
      }
      printf("Case #%d: %lld\n",cs,ring);
  }
}
