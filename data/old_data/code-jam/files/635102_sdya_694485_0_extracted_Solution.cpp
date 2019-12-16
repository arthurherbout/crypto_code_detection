#include <iostream>
#include <string>
#include <string.h>
#include <cstring>
#include <vector>
#include <algorithm>
#include <set>
#include <map>

using namespace std;

char c[1000][1000];
int n;

bool eq(char s1, char s2)
{
    if (s1 == s2) return true;
    if (s1 == ' ' || s2 == ' ') return true;
    return false;
}

int check(int x, int y)
{
    bool good = true;
    for (int i = x - 30; i <= x + 30 && good; i ++)
        for (int j = y - 30; j <= y + 30 && good; j ++)
            if (!eq(c[i][j], c[i][2 * y - j]) || !eq(c[i][j], c[2 * x - i][j]) || !eq(c[i][j], c[2 * x - i][2 * y - j]))
                good = false;
    if (!good) return 1000;
    if (good)
    {
        int res = 0;
        for (int i = x - 30; i <= x + 30; i ++)
            for (int j = y - 30; j <= y + 30; j ++)
                if (c[i][j] != ' ')
                {
                    int dx = abs(x - i);
                    int dy = j;
                    if (dy <= y) dy -= dx; else dy += dx;
                    int d = abs(y - dy);
                    res = max(res, 2 * ((d + 1) / 2) + (c[x][y] != ' '));
                }
                return (res == 0 ? 1000 : res);
    }
}

void solve(int test)
{
    char f[100];
    gets(f);
    n = 0;
    int p = strlen(f);
    for (int i = 0; i < p; i ++)
        n = n * 10 + f[i] - '0';
    for (int i = 0; i < 900; i ++)
        for (int j = 0; j < 900; j ++)
            c[i][j] = ' ';

    int cx = 200, cy = 200;
    for (int i = 1; i <= n; i ++)
    {
        char s[500];
        gets(s);
        int k = strlen(s);
        for (int j = 0; j < k; j ++)
            c[cx + i][cy + j] = s[j];
    }
    for (int i = 1; i < n; i ++)
    {
        char s[500];
        gets(s);
        int k = strlen(s);
        for (int j = 0; j < k; j ++)
            c[cx + n + i][cy + j] = s[j];
    }

    int best = 5000;
    for (int i = cx - 20; i <= cx + 30; i ++)
        for (int j = cy - 20; j <= cy + 30; j ++)
            best = min(best, check(i, j));

    printf("Case #%d: %d\n", test, best * best - n * n);
}

int main()
{
    freopen("input.txt","r",stdin);
    freopen("output.txt","w",stdout);

    int t;
    scanf("%d\n", &t);
    for (int i = 1; i <= t; i ++)
        solve(i);
    return 0;
}