#include<stdio.h>
#include<stdlib.h>
#include <iostream>
#include <bitset>
#include<string>
#include<sstream>
#include<map>
#include<algorithm>
using namespace std;

#include<math.h>

map<int, int> seen;
int n, ans;
int edge[1001];

int who(int u)
{
    for (int i = 0; i < n; i++)
        if (edge[i] == u && seen.find(i) == seen.end()) return 1;
    return 0;
}

void dfs(int u, int dep)
{
    int v = edge[u];    

    if (seen.find(v) == seen.end())
    {
        seen[v] = dep + 1;
        dfs(v, dep+1);
    }
    else
    {
        int prev = seen[v];
        int tmp;
        tmp = dep - prev + 1;

        
        if (tmp == 2)
        {
            tmp = dep + 1;
            v = edge[v];
            if (tmp > 3)
                tmp += who(v);
        }
        ans = max(tmp, ans);
    }
}

bool who()
{
    seen.clear();
    for (int i = 1; i <= n; i++)
    {
        if (seen.find(edge[i]) != seen.end())        
            return false;
        seen[i] = 1;
    }

    return true;
}

void solve()
{
    ans = 0;    

    scanf("%d\n", &n);
    for (int i = 1; i <= n; i++)
    {
        scanf("%d\n", &edge[i]);
    }

    if (who())
    {
        printf("%d\n", n);
        return;
    }



    for (int i = 1; i <= n; i++)
    {
        seen.clear();
        seen[i] = 0;
        dfs(i, 0);
    }
    printf("%d\n", ans);
}

int main()
{
#ifndef ONLINE_JUDGE
	freopen("data.txt","r",stdin);
    freopen("out.txt","w",stdout);
#endif

    int T;
    
    scanf("%d", &T);
    for(int i = 1; i <= T; i++)
    {
        printf("Case #%d: ", i);
        solve();
    }
    return 0;  
}
