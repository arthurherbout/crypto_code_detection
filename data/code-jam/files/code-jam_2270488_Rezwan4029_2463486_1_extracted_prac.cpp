/*
    Rezwan_4029
    AUST , CSE-25
*/

#include<stdio.h>
#include<iostream>
#include<cstring>
#include<string>
#include<vector>
#include<algorithm>
#include<cmath>
#include<map>
#include<sstream>
#include<set>
#include <queue>
#define pb push_back
#define ms(a,v) memset(a,v,sizeof a)
using namespace std;
typedef long long ll;
#define FRD freopen("in.txt", "r", stdin)
#define FWR freopen("out.txt", "w", stdout)

#define rep(i,n) for( __typeof(n)i = 0 ; i < n ; i++)
#define For(i,n) for( __typeof(n)i = 1 ; i <= n ; i++)
#define forstl(i,n) for(__typeof(n.begin())i = n.begin();i!=n.end();i++)


string toString(ll number){ stringstream ss;ss << number; return ss.str(); }


bool palin(ll n )
{
   string x = toString(n);
   int len = x.length() ;
   rep(i,len) if(x[i] != x[len-i-1]) return false;
   return true;
}

int main()
{
    //FRD ; FWR;

    ll ps[]={1,2,3,11,22,101,111,121,202,212,1001,1111,2002,10001,10101,10201,11011,11111,11211,20002,20102,100001,101101,110011,111111,200002,1000001,1001001,1002001,1010101,1011101,1012101,1100011,1101011,1102011,1110111,1111111,2000002,2001002};

    int test , cs = 1;
    scanf("%d",&test);
    ll a , b ;
    while(test--)
    {
        scanf("%lld %lld",&a,&b);
        int ans = 0 ;
        rep(i,39)
        {
            ll x = ps[i] * ps[i] ;
            if(x>=a && x <= b)ans++;
        }
        printf("Case #%d: %d\n",cs++,ans);
    }

}

