#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <string>

using namespace std;

int a[2001], b[2001], f[2001], s[2001], g[2001];

bool v[2001], findd;

int n;

void sub(int i)
{
    int j, k, o;
    if (i > n)
    {
        for(j = n; j >= 1; --j)
        {
            g[j] = 1;
            for(k = j + 1; k <= n; ++k)
                if (s[j] > s[k] && g[j] < g[k] + 1)
                    g[j] = g[k] + 1;
            if (g[j] != b[j])
                return;
        } 
        findd = true;
        return;
    }
    for(o = 1; o <= n; ++o)
    {
        if (v[o])
            continue;
        s[i] = o;
        f[i] = 1;
        for(j = 1; j < i; ++j)
            if (s[j] < s[i] && f[i] < f[j] + 1)
                f[i] = f[j] + 1;
        if (f[i] != a[i])
            continue;
        v[o] = true;
        sub(i + 1);
        if (findd)
            return;
        v[o] = false;
    }
}

int main(void)
{
    freopen("C-small-attempt0.in","r",stdin);
    freopen("C-small-attempt0.out","w",stdout);
    int U;
    int i;
    scanf("%d",&U);
    for(int T = 1; T <= U; ++T)
    {
        scanf("%d", &n);
        for(i = 1; i <= n; ++i)
            scanf("%d", a + i);
        for(i = 1; i <= n; ++i)
            scanf("%d", b + i);
        for(i = 1; i <= n; ++i)
            v[i] = false;
        findd = false;
        sub(1);
        printf("Case #%d:", T);
        for(i = 1; i <= n; ++i)
            printf(" %d", s[i]);
        puts("");
    }
    //system("PAUSE");
    return 0;
}
