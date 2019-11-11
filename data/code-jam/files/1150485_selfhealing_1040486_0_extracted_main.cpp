#include <cstdio>
#include <cstring>
#include <algorithm>
#define two(i) (1<<(i))
using namespace std;

int g[10][10];
int n,m;
int u[105],v[105];
bool end;
int a[10], ct;
void sol(int dep, int st)
{
    if(end)return;
    if(dep==n)
    {
        do
        {
            bool ok = 1;
            for(int i = 0; i < n; i++)
            {
                int tt = 0, Cnt = 0;
                for(int j = 0; j < n; j++)
                    if(g[i][j])tt|=two(a[j]);
                for(int j = 1; j <= ct; j++)
                    if(tt&two(j))Cnt++;
                if(Cnt < ct){ok=0;break;}
            }
            if(ok)end = 1;
        }while(!end && next_permutation(a, a+n));
        return;
    }
    for(int i = st; i <= min(a[dep-1]+1,ct); i++)
    {
        a[dep] = i;
        sol(dep+1, (ct-i)>=n-dep-1?i+1:i);
        if(end)break;
    }
}

int main()
{
    freopen("C-small-attempt1.in","r",stdin);
    freopen("out.txt","w",stdout);

    int T,cas=0;scanf("%d",&T);
    while(T--)
    {
        memset(g,0,sizeof(g));
        scanf("%d%d",&n,&m);
        for(int i = 0; i < m; i++)
        {
            scanf("%d",&u[i]);
            u[i]--;
        }
        for(int i = 0; i < m; i++)
        {
            scanf("%d",&v[i]);
            v[i]--;
        }
        for(int i = 0; i < m; i++)
            g[u[i]][v[i]] = g[v[i]][u[i]] = 1;
        for(int i = 0; i < n; i++)g[i][i]=1;
        for(int i = 0; i < n; i++)
            g[i][(i+1)%n] = g[(i+1)%n][i] = 1;

        end = 0;
        for(ct = n; ct >= 2; ct--)
        {
            a[0] = 1;
            sol(1, 1);
            if(end)break;
        }
        if(ct==1)for(int i = 0; i < n; i++)a[i]=1;
        printf("Case #%d: %d\n",++cas,ct);
        for(int i = 0; i < n; i++)printf("%d%c",a[i],i==n-1?'\n':' ');
    }

    return 0;
}
