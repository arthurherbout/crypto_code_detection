#include <cstdio>
#include <cstdlib>
#include <iostream>

using namespace std;

int f[1000005];
int g[1000005];
int h[1000005];
int a[105];


int main(void)
{
    freopen("A-small-attempt4.in","r",stdin);
    freopen("A-small-attempt4.out","w",stdout);
    int U;
    int aa, i, j, n, t, m;
    scanf("%d",&U);
    for(int T = 1; T <= U; ++T)
    {
        scanf("%d%d", &aa, &n);
        for(i = 0; i <= 1000001; ++i)
        {
            g[i] = 0;
            h[i] = 0;
        }
        for(i = 1; i <= n; ++i)
        {
            scanf("%d", a + i);
            ++g[a[i]];
            h[a[i]] += a[i];
        }
        for(i = 1; i <= 1000001; ++i)
        {
            g[i] += g[i - 1];
            h[i] += h[i - 1];
        }
        t = 0;
        f[0] = aa;
        while(f[0] < 1000001 && g[f[0] - 1] - g[max(t - 1, 0)] > 0)
        {
            int temp = f[0];
            f[0] += h[f[0] - 1] - h[max(t - 1, 0)];
            t = temp;
        }
        m = 0;
        int ans = 0;
        if (f[0] < 1000001)
            ans = g[1000001] - g[max(f[0] - 1, 0)];
        if (aa == 1)
            ans = n;
        else
        while(f[m] < 1000001)
        {
            t = f[m];
            f[m + 1] = f[m] + f[m] - 1;
            ++m;
            while(f[m] < 1000001 && g[f[m] - 1] - g[max(t - 1, 0)] > 0)
            {
                int temp = f[m];
                f[m] += h[f[m] - 1] - h[max(t - 1, 0)];
                t = temp;
            }
            if (f[m] >= 1000001)
                ans = min(ans, m);
            else ans = min(ans, m + g[1000001] - g[max(f[m] - 1, 0)]);
        }
        printf("Case #%d: %d\n", T, ans);
    }
    return 0;
}
