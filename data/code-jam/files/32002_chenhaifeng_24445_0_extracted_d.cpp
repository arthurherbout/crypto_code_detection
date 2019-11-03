#include <stdio.h>
#include <string.h>
int N,R,W,H;
int map[105][105];
int dp[105][105];

int main()
{
    freopen("D-small-attempt0.in","r",stdin);
    freopen("ds.out","w",stdout);
    int i,j,cas,r,c;
    scanf("%d",&N);
    for(cas=1;cas<=N;cas++)
    {
        scanf("%d%d%d",&H,&W,&R);
        memset(map,0,sizeof(map));
        memset(dp,0,sizeof(dp));
        for(i=0;i<R;i++)
        {
            scanf("%d%d",&r,&c);
            map[r][c]=1;
        }
        dp[1][1]=1;
        for(i=1;i<=H;i++)
        {
            for(j=1;j<=W;j++)
            {
                if(i-1>=1&&j-2>=1&&map[i][j]!=1)
                {
                    dp[i][j]+=dp[i-1][j-2];
                    while(dp[i][j]>=10007)dp[i][j]-=10007;
                }
                if(i-2>=1&&j-1>=1&&map[i][j]!=1)
                {
                    dp[i][j]+=dp[i-2][j-1];
                    while(dp[i][j]>=10007)dp[i][j]-=10007;
                }
            }
        }
        printf("Case #%d: %d\n",cas,dp[H][W]);
    }
}
