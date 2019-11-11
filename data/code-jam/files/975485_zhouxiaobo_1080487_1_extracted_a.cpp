#include <iostream>
#include <cstdlib>
#include <cstdio>

using namespace std;

int abs(int x)
{
    return x>=0?x:-x;
}

int main(void)
{
    freopen("A-large.in","r",stdin);
    freopen("A-large.out","w",stdout);
    char st[2];
    int t[2],p[2],n,u,tt,i,b,o,T;
    scanf("%d",&T);
    for(u=1;u<=T;++u)
    {
        scanf("%d",&n);
        tt=0;
        t[0]=t[1]=0;
        p[0]=p[1]=1;
        for(i=1;i<=n;++i)
        {
            scanf("%s",st);
            scanf("%d",&b);
            if (st[0]=='O')
                o=0;
            else o=1;
            tt=max(tt+1,t[o]+abs(b-p[o])+1);
            p[o]=b;
            t[o]=tt;
        }
        printf("Case #%d: %d\n",u,tt);
    }
    return 0;
}
