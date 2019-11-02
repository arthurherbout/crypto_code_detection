#include <iostream>
#include <cstdio>
#include <string.h>
#include <algorithm>
#include <queue>
using namespace std;
#define MAXN 100 + 10

unsigned long long a[MAXN],b[MAXN];
int ha[MAXN],hb[MAXN];
int n,m;
unsigned long long ans;
void dfs(int c,unsigned long long l,int cc,unsigned long long ll,unsigned long long cnt)
{
    unsigned long long s,tl,tll;
    if(c==n+1 || cc==m+1)
    {
        if(cnt>ans)
            ans=cnt;
        return;
    }
    if(ha[c]==hb[cc])
    {
        s=min(l,ll);
        tl=l-s;
        tll=ll-s;
        if(tl+tll==0)
            dfs(c+1,a[c+1],cc+1,b[cc+1],cnt+s);
        else if(tl==0)
            dfs(c+1,a[c+1],cc,tll,cnt+s);
        else
            dfs(c,tl,cc+1,b[cc+1],cnt+s);
    }
    else
    {
        dfs(c+1,a[c+1],cc,ll,cnt);
        dfs(c,l,cc+1,b[cc+1],cnt);
    }
}
int main()
{
    freopen("t.in","r",stdin);
    freopen("t.txt","w",stdout);
    int t,tb;
    unsigned long long ta;
    scanf("%d",&t);
    for(int z=1;z<=t;z++)
    {
        memset(a,0,sizeof(a));
        memset(b,0,sizeof(b));
        scanf("%d%d",&n,&m);
        for(int i=1;i<=n;i++)
        {
            cin>>ta>>tb;
            a[i]=ta;
            ha[i]=tb;
        }
        for(int i=1;i<=m;i++)
        {
            cin>>ta>>tb;
            b[i]=ta;
            hb[i]=tb;
        }
        ans=0;
        dfs(1,a[1],1,b[1],0);
        printf("Case #%d: ",z);
        cout<<ans<<endl;
    }
    return 0;
}
