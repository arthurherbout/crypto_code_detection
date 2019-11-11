#include <iostream>
#include <cstdio>
#include <string.h>
#include <algorithm>
#include <queue>
using namespace std;
#define MAXN 1000 + 10
#define INF 0x7fffffff
typedef pair<int,int>ele;
int n,e[MAXN][20],deg[MAXN];
bool vis[MAXN];

bool bfs(int c)
{
    queue<int>q;
    q.push(c);
    int tmp;
    while(!q.empty())
    {
        tmp=q.front();
        q.pop();
        for(int i=1;i<=deg[tmp];i++)
        {
            if(tmp==e[tmp][i])
                continue;
            if(vis[ e[tmp][i] ]==true)
                return true;
            else
            {
                vis[ e[tmp][i] ]=true;
                q.push(e[tmp][i]);
            }
        }
    }
    return false;
}

int main()
{
    freopen("tt.in","r",stdin);
    freopen("t.txt","w",stdout);
    int t;
    scanf("%d",&t);
    for(int z=1;z<=t;z++)
    {
        scanf("%d",&n);
        for(int i=1;i<=n;i++)
        {
            scanf("%d",&deg[i]);
            for(int j=1;j<=deg[i];j++)
                scanf("%d",&e[i][j]);
        }
        bool flag = false;
        for(int i=1;i<=n;i++)
        {
            memset(vis,false,sizeof(vis));
            vis[i]=true;
            if(bfs(i))
            {
                flag=true;
                break;
            }
        }
        printf("Case #%d: ",z);
        if(flag)
            printf("Yes\n");
        else
            printf("No\n");
    }
    return 0;
}
