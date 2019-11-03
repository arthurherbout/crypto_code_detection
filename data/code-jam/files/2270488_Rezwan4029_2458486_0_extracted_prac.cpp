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
#include<stack>
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

int n , k;
int currentKeys[207];
int needToOpen[25];
int keys[205];

vector<int>howManysKeys[25];
stack<int >s;
int dp[1<<20 + 5 ];
int call(int mask){

    if(mask == (1<<n)-1 ) return 1;
    if(dp[mask]!=-1) return dp[mask];
    rep(i,n){
        if(mask&1<<i) continue;
        if(keys[ needToOpen[i] ]){
            keys[ needToOpen[i] ]--;
            rep(j,(int)howManysKeys[i].size())
                keys[howManysKeys[i][j]]++;

            if(call(mask|1<<i))
               {
                   s.push(i+1);
                   return 1;
               }

            keys[needToOpen[i]]++;
            rep(j,(int)howManysKeys[i].size())
                keys[howManysKeys[i][j]]--;
        }
    }
    return dp[mask]=0;
}

int main()
{
   // FRD;FWR;
    int test , cs = 1 ;
    scanf("%d",&test);

    while(test--)
    {
        ms(dp,-1);
            ms(keys,0);
            while(!s.empty()) s.pop();

            scanf("%d %d",&k,&n);
            rep(i,k){
                scanf("%d",currentKeys+i);
                keys[currentKeys[i]]++;
            }
            rep(i,n)
            {
                scanf("%d",needToOpen+i);
                int x ,y;
                scanf("%d",&x);
                rep(z,x){
                    scanf("%d",&y);
                    howManysKeys[i].pb(y);
                }
            }
            printf("Case #%d:",cs++);
            if(call(0))
            {

                while(!s.empty()){
                      printf(" %d",s.top());
                      s.pop();

                }
                puts("");
            }
            else printf(" IMPOSSIBLE\n");
            rep(i,22)howManysKeys[i].clear();
    }
}
