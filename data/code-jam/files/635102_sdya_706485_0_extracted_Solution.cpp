#include <iostream>
#include <string>
#include <string.h>
#include <cstring>
#include <vector>
#include <algorithm>
#include <set>
#include <map>

using namespace std;

int n;
int a[500][500], b[500][500];

void solve(int test)
{
    for (int i = 0; i <= 450; i ++)
        for (int j = 0; j <= 450; j ++)
            a[i][j] = 0;

    scanf("%d", &n);
    for (int i = 1; i <= n; i ++)
    {
        int lx, ly, rx, ry;
        scanf("%d%d%d%d", &lx, &ly, &rx, &ry);
        for (int j = lx; j <= rx; j ++)
            for (int k = ly; k <= ry; k ++)
                a[j][k] = 1;
    }

    int moves = 0;
    while (true)
    {
        int p = 0;
        for (int i = 0; i <= 450; i ++)
            for (int j = 0; j <= 450; j ++)
                p += a[i][j];
        if (p == 0) break;

        moves ++;

        for (int i = 1; i <= 450; i ++)
            for (int j = 1; j <= 450; j ++)
                if (a[i][j])
                {
                    if (!a[i][j - 1] && !a[i - 1][j]) b[i][j] = 0; else b[i][j] = 1;
                } else
                {
                    if (a[i][j - 1] && a[i - 1][j]) b[i][j] = 1; else b[i][j] = 0;
                }

        for (int i = 0; i <= 450; i ++)
            for (int j = 0; j <= 450; j ++)
                a[i][j] = b[i][j];
    }

    printf("Case #%d: %d\n", test, moves);
}

int main()
{
    freopen("input.txt","r",stdin);
    freopen("output.txt","w",stdout);
    int t;
    scanf("%d", &t);
    for (int i = 1; i <= t; i ++)
        solve(i);
    return 0;
}