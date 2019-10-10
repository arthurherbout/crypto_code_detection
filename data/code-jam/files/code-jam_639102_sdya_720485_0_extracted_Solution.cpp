#include <iostream>
#include <string>
#include <string.h>
#include <cstring>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#pragma comment(linker, "/STACK:256000000")

using namespace std;

//long long d[101][1024][1024];
int n, b;
int a[20];
bool u[10][20];
int ans = 0;

void rec(int k, int s, int mx)
{
    int d[20];
    if (s == n)
    {
        ans ++;
        return;
    }

    for (int i = mx + 1; i < n; i ++)
    {
        int t = i, r = 0;
        while (t > 0)
            r ++, d[r] = t % b, t /= b;

        bool good = true;
        for (int j = 1; j <= r; j ++)
            if (u[j][d[j]]) {good = false; break;}

        if (s + i > n) good = false;

        if (good)
        {
            for (int j = 1; j <= r; j ++)
                u[j][d[j]] = true;
            a[k + 1] = i;
            rec(k + 1, s + i, i);
            for (int j = 1; j <= r; j ++)
                u[j][d[j]] = false;
        }
    }
}

void solve(int test)
{
    scanf("%d%d", &n, &b);
    ans = 1;
    memset(u, false, sizeof(u));
    rec(0, 0, 0);
    printf("Case #%d: %d\n", test, ans);
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