#include<iostream>
#include<cstring>
#include<cstdio>
#include<algorithm>
#include<cmath>
#include<stack>
#include<queue>
#include<vector>
#include<map>
#include<ctime>
#include<set>
#include<string>
using namespace std;
const int MAX=20005;
const double inf=1e20;
#define  eps 1e-6
#define ll long long
#define PB push_back
#define PII pair<int,int>
#define MP(x,y) make_pair(x,y)
int dp[12][10],v[MAX];
int E,R,N;
int main()
{
    freopen("B-small.in","r",stdin);
    freopen("out.txt","w",stdout);
    int i,j,k,T;
    scanf("%d",&T);
    for(int cas=1;cas<=T;cas++)
    {
        scanf("%d%d%d",&E,&R,&N);
        //cout<<E<<" "<<R<<" "<<N<<endl;
        memset(dp,0,sizeof(dp));
        for(i=1;i<=N;i++)
            scanf("%d",&v[i]);
        for(i=0;i<N;i++)
        {
            for(j=0;j<=E;j++)
            {
                for(k=0;k<=j;k++)
                {
                    dp[i+1][min(j-k+R,E)]=max(dp[i+1][min(j-k+R,E)],dp[i][j]+k*v[i+1]);
                }
            }
        }
        int res=0;
        for(i=0;i<=E;i++)
            res=max(res,dp[N][i]);
        printf("Case #%d: %d\n",cas,res);
    }
    return 0;
}
