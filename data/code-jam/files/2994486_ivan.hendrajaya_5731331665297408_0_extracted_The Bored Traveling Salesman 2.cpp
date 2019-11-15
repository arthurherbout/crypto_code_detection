#include <cstdio>
#include <set>
#include <string>
#include <vector>
#include <cstring>
#include <utility>

#define fi first
#define sc second
#define mp make_pair
#define pb push_back

using namespace std;

char zip[55][10], res[255];
bool visit[55];
vector <int> graph[55];

void dfs(int u)
{
    int i;
    set < pair <string, int> > memo;
    set < pair <string, int> > :: iterator it;
    visit[u]=1;
    strcat(res, zip[u]);
    for (i=0;i<graph[u].size();i++)
        memo.insert(mp(zip[graph[u][i]], graph[u][i]));
    for (it=memo.begin();it!=memo.end();it++)
        if (!visit[it->sc])
            dfs(it->sc);
}

int main(void)
{
    int t, n, m, i, u, v, s, tc=0;
    for (scanf("%d", &t);t--;)
    {
        scanf("%d%d", &n, &m);
        s=0;
        for (i=0;i<n;i++)
        {
            scanf("%s", zip[i]);
            if (strcmp(zip[i], zip[s])<0)
                s=i;
            visit[i]=0;
            graph[i].clear();
        }
        for (;m--;)
        {
            scanf("%d%d", &u, &v);
            u--; v--;
            graph[u].pb(v);
            graph[v].pb(u);
        }
        memset(res, 0, sizeof(res));
        visit[s]=1;
        dfs(s);
        printf("Case #%d: %s\n", ++tc, res);
    }
    return 0;
}
