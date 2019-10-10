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

int v[107] , n , e, r;

int ans ;
void solve(int pos , int energy , int gain){
    if(pos == n){
        ans = max( ans , gain);
        return ;
    }

    for(int i = 0 ; i <= energy ; i++){
        if(energy - i + r <= e)
            solve(pos+1,energy-i+r,gain+v[pos]*i );
        else
            solve(pos+1,e,gain+v[pos]*i);
    }
}
int main()
{
    //FRD ; FWR;
    int test = II ;
    For(cs,test){
         e = II , r = II , n = II ;
        rep(i,n) v[i] = II ;

        ans = 0 ;
        solve(0,e,0);
        printf("Case #%d: %d\n",cs,ans);
    }
}
