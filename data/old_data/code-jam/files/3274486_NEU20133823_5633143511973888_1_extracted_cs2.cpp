#include<bits/stdc++.h>
using namespace std;
double p[105];
double dp[105][105];
int main()
{
    freopen("C-small-2-attempt2.in","r",stdin);
    freopen("c-s2.out","w",stdout);
    int t;
    scanf("%d",&t);
    for(int cases=1;cases<=t;cases++)
    {
        int n,k;
        double u;
        scanf("%d%d%lf",&n,&k,&u);
        //printf("%d %d\n%lf\n",n,k,u);
        for(int i=0;i<n;i++)
        {
            scanf("%lf",&p[i]);
        }
        for(int i=0;i<n;i++)
        {
            //printf("%lf ",p[i]);
        }
        //printf("\n");
        sort(p,p+n);
        for(int i=n-1;i>=0;i--)
        {
            if(p[i]+u>1)
            {
                u=u-(1-p[i]);
                p[i]=1;
            }
            else
            {
                p[i]=p[i]+u;
                break;
            }
        }
        memset(dp,0,sizeof(dp));
        dp[0][0]=1.0;
        for(int i=1;i<=n;i++)
        {
            //double yes=p[n-i],no=1.0-p[n-i];
            double yes=p[i-1],no=1.0-p[i-1];
            dp[i][0]=dp[i-1][0]*no;
            for(int j=1;j<=i;j++)
            {
                dp[i][j]=dp[i-1][j-1]*yes+dp[i-1][j]*no;
            }
        }
        double ans=0.0;
        for(int i=k;i<=n;i++)
        {
            ans=ans+dp[n][i];
        }
        printf("Case #%d: %lf\n",cases,ans);
    }
    return 0;
}

