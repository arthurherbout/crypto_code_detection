#include <iostream>
#include <cstdlib>
#include <cstdio>

using namespace std;

struct chest
{
   int t, k;
   int f[401];
}c[201];

int n, k;
int s[201];
int res[201];

bool check(int x, int t)
{
    int res = s[t];
    for(int i = 1; i <= n; ++i)
        if (((x >> (i - 1)) & 1))
        {
            if (c[i].t == t)
                --res;
            for(int j = 1; j <= c[i].k; ++j)
                if (c[i].f[j] == t)
                    ++res;
        }
    return res > 0;
}

bool opt[1<<20];
int g[1<<20];

bool cmp(int u, int v)
{
    while((u ^ (1 << (g[u] - 1))) != (v ^ (1 << (g[v] - 1))))
    {
        u ^= 1 << (g[u] - 1);
        v ^= 1 << (g[v] - 1);
    }
    return g[u] < g[v];
}

int main(void)
{
    freopen("D-small-attempt2.in", "r", stdin);
    freopen("D-small-attempt2.out", "w", stdout);
    int T, U, i, j, x, y;
    scanf("%d", &T);
    for(U = 1; U <= T; ++U)
    {
        for(i = 1; i <= 200; ++i)
            s[i] = 0;
        scanf("%d%d", &k, &n);
        for(j = 1; j <= k; ++j)
        {
            scanf("%d", &x);
            ++s[x];
        }
        for(i = 1; i <= n; ++i)
        {
            scanf("%d%d", &c[i].t, &c[i].k);
            for(j = 1; j <= c[i].k; ++j)
                scanf("%d", &c[i].f[j]);
        }
        for(x = 0; x < (1<<n); ++x)
            opt[x] = false;
        opt[0] = true;
        for(x = 0; x + 1 < (1<<n); ++x)
        {
            if (!opt[x])
                continue;
            for(i = 1; i <= n; ++i)
            {
                y = (x | (1 << (i - 1)));
                if (x == y)
                    continue;
                if (check(x, c[i].t))
                if (!opt[y] || cmp(x, (y ^ (1 << (g[y] - 1)))))
                {
                    opt[y] = true;
                    g[y] = i;
                }
            }
        }
        printf("Case #%d:", U);
        x = (1<<n) - 1;
        if (opt[x])
        {
            for(i = n; i >= 1; --i)
            {
                res[i] = g[x];
                x ^= (1 << (g[x] - 1));
            }
            for(i = 1; i <= n; ++i)
                printf(" %d", res[i]);
            puts("");
        }
        else
            puts(" IMPOSSIBLE");
        
    }
    return 0;
}
