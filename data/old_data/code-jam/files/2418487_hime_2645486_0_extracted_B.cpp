#include<cstdio>
#include<algorithm>
#include<cstring>

#define MAXX 7

short T,ca;
int e,n,r,i,j,k,l,v[133];
int dp[133][77];

int main()
{
    scanf("%hd",&T);
    for(ca=1;ca<=T;++ca)
    {
        scanf("%d %d %d",&e,&r,&n);
        r=std::min(r,e);
        memset(dp,0xc0,sizeof(dp));
        dp[0][e]=0;
        for(i=1;i<=n;++i)
        {
            scanf("%d",v+i);
            for(j=r;j<=e;++j)
                if(dp[i-1][j]>=0)
                    for(k=0;k<=j;++k)
                    {
                        l=std::min(e,j-k+r);
                        dp[i][l]=std::max(dp[i][l],dp[i-1][j]+v[i]*k);
                    }
        }
        for(j=0,i=r;i<=e;++i)
            j=std::max(j,dp[n][i]);
        printf("Case #%hd: %d\n",ca,j);
    }
    return 0;
}
