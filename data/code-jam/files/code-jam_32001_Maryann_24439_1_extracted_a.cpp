#include<cstdio>
#include<algorithm>
#define oo 1013741823
using namespace std;
int n,m,v,ans;
int g[10000],c[10000];
int d[10000][2];
int dp(int x,int val)
{
    if(d[x][val]!=-1)return d[x][val];
    if(x<=n)
    {
        d[x][val]=oo;
        if(g[x])//AND
        {
            if(val)
            d[x][val]=min(d[x][val],dp(x*2,1)+dp(x*2+1,1));
            else
            d[x][val]=min(d[x][val],min(min(dp(x*2,0)+dp(x*2+1,1),dp(x*2,1)+dp(x*2+1,0)),dp(x*2,0)+dp(x*2+1,0)));
        }
        else
        {
            if(val)
            d[x][val]=min(d[x][val],min(min(dp(x*2,0)+dp(x*2+1,1),dp(x*2,1)+dp(x*2+1,0)),dp(x*2,1)+dp(x*2+1,1)));
            else
            d[x][val]=min(d[x][val],dp(x*2,0)+dp(x*2+1,0));
        }
        if(c[x])
        {
        if(!g[x])//modify to AND
        {
            if(val)
            d[x][val]=min(d[x][val],dp(x*2,1)+dp(x*2+1,1)+1);
            else
            d[x][val]=min(d[x][val],min(min(dp(x*2,0)+dp(x*2+1,1),dp(x*2,1)+dp(x*2+1,0)),dp(x*2,0)+dp(x*2+1,0))+1);
        }
        else
        {
            if(val)
            d[x][val]=min(d[x][val],min(min(dp(x*2,0)+dp(x*2+1,1),dp(x*2,1)+dp(x*2+1,0)),dp(x*2,1)+dp(x*2+1,1))+1);
            else
            d[x][val]=min(d[x][val],dp(x*2,0)+dp(x*2+1,0)+1);
        }
        }
    }
    else
    d[x][val]=(g[x]==val?0:oo);
    return d[x][val];
}
int main()
{
    int _,t;
    scanf("%d",&_);
    for(t=1; t<=_; t++)
    {
        scanf("%d%d",&m,&v);
        n=m/2;
        for(int i=1; i<=n; i++)
        scanf("%d%d",&g[i],&c[i]);
        for(int i=n+1; i<=m; i++)
        scanf("%d",&g[i]);
        memset(d,-1,sizeof(d));
        ans=dp(1,v);
        printf("Case #%d: ",t);
        if(ans==oo)
        puts("IMPOSSIBLE");
        else
        printf("%d\n",ans);
    }
    return 0;
}