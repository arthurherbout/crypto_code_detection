#include <stdio.h>
#include <iostream>
#include <string.h>
using namespace std;
#define INF (1<<30)
#define MAXN 1010
int L, t, N, C;
int ans, s, ts;
int a[MAXN];
int d[MAXN];

int MAX(int x, int y)
{
    return x > y ? x : y;
}
int MIN(int x, int y)
{
    return x < y ? x : y;
}

int cal1()
{
    int i, ns;
    ans = d[N] * 2;
    for (i = N; i > 0 && i > N - C; i--)
    {
        if (d[i] * 2 <= t)
        {
            ans = MIN(ans, d[N] * 2);
            //printf ("ans %d i=%d\n", ans, i);
            return ans;
        }
        ns = d[i] - MAX(d[i-1], (t / 2));

        ans = MIN(ans, ns + (d[N] - ns) * 2);
        //printf ("ans %d i=%d ns=%d d[i-1]=%d t/2=%d\n", ans, i, ns, d[i-1], t/2);
    }
    return ans;
}

int cal2()
{
    int i, j, t0, t1, ns;

    for (i = 1; i < N; i++)
    {
        if (d[i] * 2 <= t)
        {
            t0 = d[i] * 2;
        }
        else
        {
            ns = d[i] - MAX(d[i-1], (t / 2));
            t0 = ns + (d[i] - ns) * 2;
        }

        for (j = i + 1; j <= N; j++)
        {
            if (t0 + (d[j] - d[i]) * 2 <= t)
                t1 = d[N] * 2;
            else
            {
                ns = d[j] - d[i] - MAX(d[j-1]-d[i], (t-t0) / 2);
                t1 = t0 + ns + (d[N] - d[i] - ns) * 2;
            }
            //printf ("i=%d j=%d t1=%d\n", i, j, t1);
            ans = MIN(ans, t1);
        }
    }
    return ans;
}

void pr(int a[])
{
    int i;
    for (i = 0; i <= N; i++)
        printf ("%d: %d\n", i, a[i]);
    puts("");
}

int main()
{
    freopen("B-small-attempt0 (1).in", "r", stdin);
    freopen("1.out", "w", stdout);

    int i, j, cs, csnum;
    int ns;

    scanf ("%d", &csnum);

    for (cs = 1; cs <= csnum; cs++)
    {
        scanf ("%d %d %d %d", &L, &t, &N, &C);

        for (i = 1; i <= C; i++)
        {
            scanf (" %d", &a[i]);
        }
        a[0] = a[C];

        d[0] = 0;
        for (i = 1; i <= N; i++)
            d[i] = d[i-1] + a[i%C];

        //printf ("s=%d\n", s);
        //puts("d");
        //pr(d);


        ans = d[N] * 2;
        if (L == 0)
        {
            ans = 2 * d[N];
        }
        else if (L == 1)
        {
            cal1();
        }
        else cal2();
        printf ("Case #%d: %d\n", cs, ans);
    }

}
/*
5
1 4 4 2 10 4
*/
