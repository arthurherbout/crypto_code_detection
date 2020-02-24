#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<algorithm>
using namespace std;
const int oo=1000000;
int T,C,n,l,i,ans;
int p[100];
char a[100][100],b[100][100];
bool used[100],c[20];

void dfs(int u)
{
    int i,j,tp;
    if (u==n+1)
    {
        //for (i=1;i<=n;i++) cout<<a[i]<<" "<<b[p[i]]<<endl; cout<<endl;
        tp=0;
        memset(c,false,sizeof(c));
        for (j=0;j<l;j++)
        if (a[1][j]!=b[p[1]][j])
        {
            c[j]=true;
            tp++;
        }
        for (i=1;i<=n;i++)
        {
            for (j=0;j<l;j++)
            {
                if (c[j]&&a[i][j]==b[p[i]][j]) break;
                if ((!c[j])&&a[i][j]!=b[p[i]][j]) break;
            }
            if (j<l) break;
        }
        if (i>n&&j>=l) ans=min(ans,tp);
        return;
    }
    for (i=1;i<=n;i++)
    if (!used[i])
    {
        used[i]=true;
        p[u]=i;
        dfs(u+1);
        used[i]=false;
    }
}
int main()
{
    freopen("1.in","r",stdin);
    freopen("1.out","w",stdout);
    scanf("%d",&T);
    for (C=1;C<=T;C++)
    {
        scanf("%d%d",&n,&l);
        for (i=1;i<=n;i++) scanf("%s",a[i]);
        for (i=1;i<=n;i++) scanf("%s",b[i]);
        memset(used,false,sizeof(used));
        ans=oo;
        dfs(1);
        printf("Case #%d: ",C);
        if (ans==oo) printf("NOT POSSIBLE\n");
        else printf("%d\n",ans);
    }

    return 0;
}
