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


#define sz 107

int M[sz][sz];
int mROW[sz];
int mCOL[sz];

int main()
{
   // FRD ; FWR;
    int test , cs = 1 ;
    scanf("%d",&test);
    int n , m ;
    while(test--)
    {
        scanf("%d %d",&n,&m);

        rep(i,n) rep(j,m) scanf("%d",&M[i][j]);

        rep(i,n)
        {
            int mx = M[i][0];
            rep(j,m) if (M[i][j]>mx)   mx = M[i][j];
            mROW[i] = mx ;
        }

        rep(j,m)
        {
            int mx = M[0][j];
            rep(i,n) if (M[i][j]>mx)   mx = M[i][j];
            mCOL[j] = mx ;
        }

        bool flag = true;

        rep(i,n)
        {
            rep(j,m)
            {
                if ( M[i][j] < mROW[i] && M[i][j] < mCOL[j] ){
                    flag = false;
                    break;
                }
            }
            if(!flag)break;
        }

         printf ("Case #%d: %s\n",cs++,flag?"YES":"NO");
    }
}
