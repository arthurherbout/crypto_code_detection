#include<cstdio>
#include<memory>

int h,i,j,k,l,u,n,m,t,p[65536],q[65536];
char a[55][55];

int main()
{
    freopen("e.in","r",stdin);
    freopen("e.out","w",stdout);
    scanf("%d",&t);
    for(h=1;h<=t;h++)
    {
        memset(a,0,sizeof(a));
        scanf("%d%d",&n,&m);
        for(i=1;i<=n;i++)
        {
            scanf(" %s",&a[i][1]);
            a[i][0]='.';
        }
        for(i=0;i<=m;i++)a[0][i]='.';
        memset(p,-1,sizeof(p));
        p[0]=0;
        for(i=1;i<=n;i++)
        for(j=1;j<=m;j++)
        {
            memset(q,-1,sizeof(q));
            for(k=0;k<(2<<m);k++)
            if(p[k]>-1)
            {
                if(a[i][j]=='.')
                {
                    u=k;
                    if((1<<j)&u)u-=(1<<j);
                    if(q[u]<0||p[k]>q[u])q[u]=p[k];
                }
                if(a[i][j]=='#')
                {
                    u=k;
                    if(((1<<j)&u)==0)u+=(1<<j);
                    l=4;
                    if((1<<j)&k)l-=2;
                    if((1<<(j-1))&k)l-=2;
                    if(q[u]<0||p[k]+l>q[u])q[u]=p[k]+l;
                }
                if(a[i][j]=='?')
                {
                    u=k;
                    if((1<<j)&u)u-=(1<<j);
                    if(q[u]<0||p[k]>q[u])q[u]=p[k];
                    u=k;
                    if(((1<<j)&u)==0)u+=(1<<j);
                    l=4;
                    if((1<<j)&k)l-=2;
                    if((1<<(j-1))&k)l-=2;
                    if(q[u]<0||p[k]+l>q[u])q[u]=p[k]+l;
                }
            }
            memcpy(p,q,sizeof(p));
        }
        u=0;
        for(k=0;k<(2<<m);k++)
            if(p[k]>u)u=p[k];
        printf("Case #%d: %d\n",h,u);
    }
    return 0;
}
