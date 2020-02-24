#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<algorithm>
using namespace std;
const int oo=1000000;
const int size=1005*2;
int T,C,i,n,ans,tp,xx,yy,tot;
int head[size],next[size],y[size],f[size],d[size];


void add(int xx,int yy)
{
    y[++tot]=yy;
    next[tot]=head[xx]; head[xx]=tot;
}

void dfs(int u,int fa)
{
    int s=0;
    f[u]=1;
    for (int go=head[u];go;go=next[go])
    if (y[go]!=fa)
    {
        dfs(y[go],u);
        f[u]+=f[y[go]];
        s++;
    }

    if (s==1)
    {
        for (int go=head[u];go;go=next[go])
        if (y[go]!=fa)
        {
            tp+=f[y[go]];
            f[u]-=f[y[go]];
        }
    }

    if (s>2)
    {
        int pp=0,ss=0;
        for (int go=head[u];go;go=next[go])
        if (y[go]!=fa)
        {
            d[++pp]=f[y[go]];
            ss+=f[y[go]];
        }
        sort(d+1,d+pp+1);
        tp+=ss-d[pp]-d[pp-1];
        f[u]-=ss-d[pp]-d[pp-1];
    }
    //cout<<u<<" "<<f[u]<<endl;
}

int main()
{
    freopen("1.in","r",stdin);
    freopen("1.out","w",stdout);
    scanf("%d",&T);
    for (C=1;C<=T;C++)
    {
        memset(head,0,sizeof(head)); tot=0;
        scanf("%d",&n);
        for (i=1;i<n;i++)
        {
            scanf("%d%d",&xx,&yy);
            add(xx,yy);
            add(yy,xx);
        }
        ans=oo;
        for (i=1;i<=n;i++)
        {
            tp=0;
            dfs(i,0);
            ans=min(ans,tp);
        }
        printf("Case #%d: %d\n",C,ans);
    }

    return 0;
}
