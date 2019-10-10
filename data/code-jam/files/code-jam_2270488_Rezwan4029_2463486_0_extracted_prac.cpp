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


string toString(int number){ stringstream ss;ss << number; return ss.str(); }

int isPal[1007] = {0};
int sum [1007] = {0};
bool palin(int n )
{
   string x = toString(n);
   int len = x.length() ;
   rep(i,len) if(x[i] != x[len-i-1]) return false;
   return true;
}
int check(int n)
{
    int sq = sqrt(n);
    if( sq * sq != n ) return 0;

    if( isPal[n] && isPal[sq] ) return 1;
    return 0;
}
void generate()
{
   For(i,1000) isPal[ i ] = palin(i);
   For(i,1000) sum[i] = sum[i-1] + check(i);
}
int main()
{
    //FRD;FWR;
    generate();
    int test , cs = 1;
    scanf("%d",&test);
    while(test--)
    {
        int a , b ;
        scanf("%d %d",&a,&b) ;
        if(a > b ) swap(a,b);
        printf("Case #%d: %d\n",cs++,sum[b] - sum[a-1]);
    }
}
