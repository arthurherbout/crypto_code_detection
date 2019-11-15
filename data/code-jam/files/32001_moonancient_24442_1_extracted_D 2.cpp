#include <iostream>
#include <algorithm>
using namespace std;
#define MAXM 16
#define MAXN 50010

int n,m;
char str[MAXN];

int adj[MAXM][MAXM];
int pen[MAXM][MAXM];

int f[1<<MAXM][MAXM][MAXM];

int F(int s,int u,int v)
{
    if (f[s][u][v]<0)
    {
        f[s][u][v]=0x7FFFFFFF;
        for (int w=0;w<m;w++)
        if (w!=v && (s & (1<<w)))
        f[s][u][v]=min(f[s][u][v],F(s^(1<<v),u,w)+adj[w][v]);
    }
    return f[s][u][v];
}

int ans;

int t;
int main()
{
freopen("D.in","r",stdin);
freopen("D.out","w",stdout);

    cin>>t;
    for (int i=1;i<=t && printf("Case #%d: ",i);i++)
    {
        cerr<<i<<endl;
        scanf("%d%s",&m,str);
        n=strlen(str);
        memset(adj,0,sizeof(adj));
        memset(pen,0,sizeof(pen));
        for (int i=0;i<m;i++)
        for (int j=0;j<m;j++)
        if (i!=j)
        {
            for (int k=0;k<n;k+=m)
            if (str[k+i]!=str[k+j]) adj[i][j]++;
            for (int k=m;k<n;k+=m)
            if (str[k+i]!=str[k-m+j]) pen[i][j]++;
        }
        memset(f,-1,sizeof(f));
        for (int i=0;i<m;i++)
        {
            for (int j=0;j<(1<<m);j++) f[j][i][i]=MAXN;
            f[1<<i][i][i]=0;
        }
        ans=0x7FFFFFFF;
        for (int i=0;i<m;i++)
        for (int j=0;j<m;j++)
        if (i!=j) ans=min(ans,F((1<<m)-1,i,j)+pen[i][j]);
        cout<<ans+1<<endl;
    }
    return 0;
}
