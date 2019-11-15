#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <queue>
#include <algorithm>
#include <map>
#include <cmath>
#include <set>
#include <iomanip>

using namespace std;

typedef long long ll;
typedef vector<int> vi;
typedef pair<int, int> pii;

#define fi first
#define se second
#define mp make_pair
#define pb push_back

const int INF = 1 << 30;
const double EPS = 1e-8;

int dp[204][204][1042];

void solve(int num)
{
    for(int i=0; i<203; i++)
    {
        for(int j=0; j<203; j++)
        {
            for(int k=0; k<1040; k++) dp[i][j][k]=-INF;
        }
    }
    int n, p, q;
    cin>>p>>q>>n;
    vi h(n+2, 201), g(n);
    for(int i=0; i<n; i++) cin>>h[i]>>g[i];
    dp[0][h[0]][0]=0;
    int ans=0;
    for(int i=0; i<n; i++)
    {
        for(int hp=200; hp>0; hp--)
        {
            for(int s=0; s<=1000; s++)
            {
                if(dp[i][hp][s]<0) continue;
                //cout<<"osiagam "<<i<<" "<<hp<<" "<<s<<" z "<<dp[i][hp][s]<<endl;
                // hold
                if(hp>q)
                {
                    dp[i][hp-q][s+1]=max(dp[i][hp-q][s+1], dp[i][hp][s]);
                }
                else
                {
                    dp[i+1][h[i+1]][s+1]=max(dp[i+1][h[i+1]][s+1], dp[i][hp][s]);
                }
                // last hit
                if((s+1)*p<hp) continue;
                int s2=s+1-(hp+p-1)/p;
                if(h[i+1]>q)
                {
                    dp[i+1][h[i+1]-q][s2]=max(dp[i+1][h[i+1]-q][s2],dp[i][hp][s]+g[i]);
                }
                else
                {
                    dp[i+2][h[i+2]][s2]=max(dp[i+2][h[i+2]][s2],dp[i][hp][s]+g[i]);
                }
                if(s*p<hp) continue;
                s2=s-(hp+p-1)/p;
                dp[i+1][h[i+1]][s2]=max(dp[i+1][h[i+1]][s2],dp[i][hp][s]+g[i]);
            }
        }
    }
    for(int hp=0; hp<202; hp++)
    {
        for(int s=0; s<1002; s++) ans=max(ans, dp[n][hp][s]);
    }
    cout<<"Case #"<<num<<": "<<ans<<"\n";

}

int main()
{
    ios_base::sync_with_stdio(0);
    int t;
    cin>>t;
    for(int i=1; i<=t; i++)
    {
        solve(i);
    }
}

