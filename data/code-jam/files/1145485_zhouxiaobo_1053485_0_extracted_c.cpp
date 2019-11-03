#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>

using namespace std;

int c[110],s[110],t[110];
int m,u,f[110][110],ans;

bool cmp(const int &x,const int &y)
{
    if (c[x]>c[y]) return true;
    if (c[x]<c[y]) return false;
    return s[x]>s[y];
}

void sub(int o,int n,int score,int turn,int g)
{
    if (turn==0||n==0)
    {
        ans=max(ans,score);
        return;
    }
    int i,j,d,fscore,fturn,nk=n,gk=g;
    for(d=1;d<=n;++d)
    {
        if (d>1&&c[f[o-1][d]]<=c[f[o-1][1]]&&s[f[o-1][d]]<=s[f[o-1][1]])
            continue;
        if (d>1&&u>43)
            continue;
        fscore=score; fturn=turn;
        for(i=1;i<=n;++i)
            f[o][i]=f[o-1][i];
        for(i=1;i<=n;++i)
            if (t[f[o][i]]>0||i==d)
            {
                fturn+=t[f[o][i]]-1;
                fscore+=s[f[o][i]];
                for(j=1;j<=c[f[o][i]];++j)
                    if (g<=m)
                        f[o][++n]=(g++);
                f[o][i]=0;
            }
        sort(f[o]+1,f[o]+n+1,cmp);
        while((n>0)&&f[o][n]==0)
            --n;
        sub(o+1,n,fscore,fturn,g);
        n=nk;
        g=gk;
    }
}

int main(void)
{
    freopen("C-small-attempt2.in","r",stdin);
    freopen("C-small-attempt2.out","w",stdout);
    ans=0;
    int n,g,T,i,j,score,turn;
    scanf("%d",&T);
    for(u=1;u<=T;++u)
    {
        scanf("%d",&n);
        score=0; turn=1; ans=0;
        for(i=1;i<=n;++i)
            scanf("%d%d%d",c+i,s+i,t+i);
        scanf("%d",&m);
        m+=n;
        s[0]=c[0]=-1;
        for(i=n+1;i<=m;++i)
            scanf("%d%d%d",c+i,s+i,t+i);
        g=n+1;
        for(i=1;i<=n;++i)
        {
            f[0][i]=i;
            if (t[f[0][i]]>0)
            {
                turn+=t[f[0][i]]-1;
                score+=s[f[0][i]];
                for(j=1;j<=c[f[0][i]];++j)
                    if (g<=m)
                        f[0][++n]=(g++);
                f[0][i]=0;
            }
        }
        sort(f[0]+1,f[0]+n+1,cmp);
        while((n>0)&&f[0][n]==0)
            --n;
        sub(1,n,score,turn,g);
        printf("Case #%d: %d\n",u,ans);
    }
    return 0;
}
