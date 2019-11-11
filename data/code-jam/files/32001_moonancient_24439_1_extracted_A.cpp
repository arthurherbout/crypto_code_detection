#include <iostream>
#include <algorithm>
using namespace std;

#define MAXN 10010
int n,v;
int nodes[MAXN];
int change[MAXN];
int f[MAXN][2];
bool flag[MAXN];

void Calc(int u)
{
    if (flag[u])
    {
        int v,w;
        flag[u]=false;
        v=u*2+1;
        w=u*2+2;
        Calc(v);
        Calc(w);
        f[u][0]=f[u][1]=n+1;
        for (int i=0;i<2;i++)
        if (f[v][i]!=-1)
        for (int j=0;j<2;j++)
        if (f[w][j]!=-1)
        {
            int now;
            if (nodes[u]) now=i&j;
            else now=i|j;
            f[u][now]=min(f[u][now],f[v][i]+f[w][j]);
            if (change[u])
            {
                if (nodes[u]) now=i|j;
                else now=i&j;
                f[u][now]=min(f[u][now],f[v][i]+f[w][j]+1);
            }
        }
        if (f[u][0]>n) f[u][0]=-1;
        if (f[u][1]>n) f[u][1]=-1;
    }
}

int t;
int main()
{
freopen("A.in","r",stdin);
freopen("A.out","w",stdout);
    cin>>t;
    for (int i=1;i<=t && printf("Case #%d: ",i);i++)
    {
        int i;
        cin>>n>>v;
        for (i=0;i<n;i++)
        {
            if (i*2+1<n)
            {
                scanf("%d%d",&nodes[i],&change[i]);
                flag[i]=true;
            }
            else
            {
                int x;
                scanf("%d",&x);
                flag[i]=false;
                f[i][x]=0;
                f[i][x^1]=-1;
            }
        }
        Calc(0);
        if (f[0][v]!=-1) printf("%d\n",f[0][v]);
        else puts("IMPOSSIBLE");
    }
    return 0;
}
