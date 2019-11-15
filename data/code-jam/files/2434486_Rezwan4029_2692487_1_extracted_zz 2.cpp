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
#define CS printf("Case #%d: ",cs++)
#define ms(a,v) memset(a,v,sizeof a)
#define II ({int a; scanf("%d", &a); a;})
#define LL ({ll  a; scanf("%lld", &a); a;})
#define FRD freopen("in.txt", "r", stdin)
#define FWR freopen("out.txt", "w", stdout)
using namespace std;
typedef  long long ll;

#define rep(i,n) for( __typeof(n)i = 0 ; i < n ; i++)
#define For(i,n) for( __typeof(n)i = 1 ; i <= n ; i++)
#define forstl(i,n) for(__typeof(n.begin())i = n.begin();i!=n.end();i++)
#define forab(i,a,b)for(__typeof(b)i = a ; i <= (b) ; i++)

int cs = 1 ;

string toString(ll number){ stringstream ss;ss << number; return ss.str(); }
bool isVowel(char s){s=tolower(s); if(s=='a'||s=='e'||s=='i'||s=='o'||s=='u')return true;return false;}
ll  GCD(ll  a,ll  b){ while(b){b ^= a ^=b ^= a %=b;}  return a;}
ll  LCM(ll  a,ll  b) { return (a / GCD(a,b)*b); }
ll  Pow(ll  B,ll  P){ll  R=1; while(P>0)  {if(P&1) R=(R*B);P/=2;B=(B*B);}return R;} //compute b^p
ll  BigMod(ll  B,ll  P,ll  M){ ll  R=1; while(P>0)  {if(P&1){R=(R*B)%M;}P/=2;B=(B*B)%M;} return (ll )R;} //compute b^p%m

ll A , n ;
ll item[107];

int main()
{
   // FRD;FWR;
    int test = II ;
    while(test--)
    {
        A = LL , n = LL ;
        rep(i,n) item[i] = LL ;
        sort(item,item+n);
        ll curr = A  , cnt = 0 ;
        if(A == 1 ){
            CS ; cout << n << endl;
            continue;
        }
        ll ans = n ;
        rep(i,n)
        {
            if( item[i] < curr) curr += item[i];
            else
            {
                while(item[i] >= curr) cnt++ , curr = 2 * curr - 1 ;

                curr += item[i];
            }
            ans = min( ans , cnt+n-i-1  );
        }
        CS ; cout << ans << endl;
    }
}
