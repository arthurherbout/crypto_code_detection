#include <iostream>
#include <string>
#include <string.h>
#include <cstring>
#include <algorithm>
#include <map>
#include <set>
#include <vector>

using namespace std;

int D, I, M, N;
int d[200][300], a[200];

/*void solve(int test)
{
    scanf("%d%d%d%d", &D, &I, &M, &N);
    for (int i = 1; i <= N; i ++)
        scanf("%d", &a[i]);

    for (int i = 1; i <= N; i ++)
        for (int j = 0; j <= 255; j ++)
            d[i][j] = 1000000000;

    for (int i = 0; i <= 255; i ++)
        d[1][i] = abs(a[i] - i);
    for (int i = 0; i <= 255; i ++)
        d[1][i] = min(d[1][i], D);
    for (int i = 0; i <= 255; i ++)
    {
        if (abs(i - a[i]) <= M) d[1][i] = min(d[1][i], I); else
        {
            int t = 1000000000;
            if (i - M >= 0) t = abs(a[i] - (i - M));
            if (i + M <= 255) t = min(t, abs(a[i] - (i + M)));
            d[1][i] = min(d[1][i], t + I);
        }
        int t = abs(i - a[i]);
        int p = (t + (M - 1)) / M;
        d[1][i] = min(d[1][i], p * I);
    }

    for (int i = 2; i <= N; i ++)
        for (int j = 0; j <= 255; j ++)
        {
            for (int k = max(0, j - M); k <= min(255, j + M); k ++)
                d[i][j] = min(d[i][j], d[i - 1][k] + abs(j - a[i]));
            d[i][j] = min(d[i][j], d[i - 1][j] + D);
            for (int k = 0; k <= 255; k ++)
            {
                if (abs(j - k) <= 2 * M) d[i][j] = min(d[i][j], d[i - 1][k] + I + abs(j - a[i]));
                if (abs(j - k) <= 2 * M)
                {
                    int l = max(j, k) - M;
                    int r = min(j, k) + M;
                    if (a[i] >= l && a[i] <= r) d[i][j] = min(d[i][j], d[i - 1][k] + I);
                    d[i][j] = min(d[i][j], d[i - 1][k] + I + min(abs(l - a[i]), abs(r - a[i])));
                }
                int t = abs(j - k);
                int p = (t + (M - 1)) / M;
                d[i][j] = min(d[i - 1][j], p * I);
            }
        }

    int best = 1000000000;
    for (int i = 0; i <= 255; i ++)
        best = min(best, d[N][i]);
    printf("Case #%d: %d\n", test, best);
}*/

void solve(int test)
{
    scanf("%d%d%d%d", &D, &I, &M, &N);
    for (int i = 1; i <= N; i ++)
        scanf("%d", &a[i]);

    int res = 0;
    if (M == 0)
    {
        res = (N - 1) * D;
        for (int i = 0; i <= 255; i ++)
        {
            int cur = 0;
            for (int j = 1; j <= N; j ++)
                cur += abs(a[j] - i);
            res = min(res, cur);
        }
        if (N == 3)
        {
            for (int i = 0; i <= 255; i ++)
                res = min(res, abs(a[1] - i) + abs(a[2] - i) + D);
            for (int i = 0; i <= 255; i ++)
                res = min(res, abs(a[1] - i) + abs(a[3] - i) + D);
            for (int i = 0; i <= 255; i ++)
                res = min(res, abs(a[3] - i) + abs(a[2] - i) + D);
        }
    } else
    if (N == 1) res = 0; else
    if (N == 2)
    {
        res = D;
        for (int i = 0; i <= 255; i ++)
            for (int j = 0; j <= 255; j ++)
            {
                int t = abs(a[1] - i) + abs(a[2] - j);
                int p = abs(i - j);
                if (p <= M) res = min(res, t); else
                    res = min(res, t + I * ((p + M - 1) / M - 1));
            }
    } else
    {
        res = 2 * D;
        for (int x = 1; x <= 3; x ++)
            for (int y = x + 1; y <= 3; y ++)
                for (int i = 0; i <= 255; i ++)
                    for (int j = 0; j <= 255; j ++)
                    {
                        int t = D + abs(a[1] - i) + abs(a[2] - j);
                        int p = abs(i - j);
                        if (p <= M) res = min(res, t); else
                            res = min(res, t + I * ((p + M - 1) / M - 1));
                    }

        for (int i = 0; i <= 255; i ++)
            for (int j = 0; j <= 255; j ++)
            {
                if (abs(a[1] - i) + abs(a[2] - j) > res) continue;
                for (int k = 0; k <= 255; k ++)
                {
                    int t = abs(a[1] - i) + abs(a[2] - j) + abs(a[3] - k);
                    int p = abs(i - j), q = abs(j - k);
                    int add = 0;
                    if (p >= M) add += I * ((p + M - 1) / M - 1);
                    if (q >= M) add += I * ((q + M - 1) / M - 1);

                    res = min(res, t + add);
                }
            }
    }
    printf("Case #%d: %d\n", test, res);
}

int main()
{
    freopen("input.txt","r",stdin);
    freopen("output.txt","w",stdout);
    int test;
    scanf("%d", &test);
    for (int i = 1; i <= test; i ++)
        solve(i);
    return 0;
}