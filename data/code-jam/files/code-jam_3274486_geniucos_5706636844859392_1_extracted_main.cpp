#include<bits/stdc++.h>

using namespace std;

int N, a[1500], b[1500], c[1500];

int solve ()
{
    int L1 = 720, L2 = 720, ans = 0;
    for (int i=0; i<1440; i++)
        L1 -= (a[i] == 1);
    for (int i=0; i<1440; i++)
        L2 -= (a[i] == 2);
    for (int i=1; i<1440; i++)
        if (a[i] != 0 && a[i - 1] != 0)
            ans += (a[i] != a[i - 1]);
    b[0] = c[0] = 0;
    for (int i=1; i<1440; i++)
        if (a[i] == 0)
        {
            int j;
            for (j=i; j<1440; j++)
                if (a[j] != 0) break;
            j --;
            ///[i, j]
            if (a[i - 1] != a[j + 1]) ans ++;
            else
            {
                if (a[i - 1] == 1) b[++b[0]] = j - i + 1;
                else c[++c[0]] = j - i + 1;
            }
            ///
            i = j;
        }
    sort (b + 1, b + b[0] + 1);
    sort (c + 1, c + c[0] + 1);
    int cnt = b[0] + c[0], i = 1;
    while (b[i] <= L1 && i <= b[0])
        L1 -= b[i], i ++, cnt --;
    i = 1;
    while (c[i] <= L2 && i <= c[0])
        L2 -= c[i], i ++, cnt --;
    ans += 2 * cnt;
    return ans;
}

int main ()
{
freopen ("input", "r", stdin);
freopen ("output", "w", stdout);

int Tests;
scanf ("%d\n", &Tests);
for (int test_id = 1; test_id <= Tests; test_id ++)
{
    printf ("Case #%d: ", test_id);
    for (int i=0; i<1440; i++)
        a[i] = 0;
    int n, m;
    scanf ("%d %d", &n, &m);
    while (n --)
    {
        int x, y;
        scanf ("%d %d", &x, &y);
        for (int i=x; i<y; i++)
            a[i] = 1;
    }
    while (m --)
    {
        int x, y;
        scanf ("%d %d", &x, &y);
        for (int i=x; i<y; i++)
            a[i] = 2;
    }
    int ans = 1500;
    for (int t1 = 1; t1 < 3; t1++)
        for (int t2=1; t2 < 3; t2 ++)
        {
            if (a[0] != 0 && a[0] != t1) continue;
            if (a[1439] != 0 && a[1439] != t2) continue;
            int old0 = a[0], old1439 = a[1439];
            a[0] = t1, a[1439] = t2;
            int curr = solve () + (t1 != t2);
            if (curr < ans) ans = curr;
            a[1439] = old1439, a[0] = old0;
        }
    printf ("%d\n", ans);
}
return 0;
}
