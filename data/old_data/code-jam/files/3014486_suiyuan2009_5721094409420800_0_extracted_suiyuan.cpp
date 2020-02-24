#include <stdio.h>
#include <string.h>
#include <math.h>
#include <algorithm>
#include <vector>
#include <map>
#include <string>
using namespace std;
const int maxn = 1005;

int n, a[maxn], c[maxn];
int f[maxn][maxn];
vector<int>vis;

void pd(int &ret, int val)
{
    if (ret == -1 || ret > val)ret = val;
}

void init()
{
    vis.clear();
    for (int i = 1; i <= n; i++)vis.push_back(a[i]);
    sort(vis.begin(), vis.end());
    for (int i = 1; i <= n; i++)a[i] = lower_bound(vis.begin(), vis.end(), a[i]) - vis.begin() + 1;
    for (int i = 1; i <= n; i++)c[a[i]] = i;
    // sort(a + 1, a + n + 1);
    memset(f, -1, sizeof(f));
    f[0][0] = 0;
    // printf("%d %d\n", c[1], c[2]);

    for (int i = 0; i <= n; i++)
        for (int j = 0; j <= n; j++)
        {
            if (f[i][j] == -1)continue;
            if (i + j + 1 > n)continue;
            if (i < n)pd(f[i + 1][j], f[i][j] + abs(i + 1 - c[i + j + 1]));
            if (n - j > 0)pd(f[i][j + 1], f[i][j] + abs(n - j - c[i + j + 1]));
        }
    //printf("**%d %d %d\n", f[0][1], f[1][0], f[1][1]);
}

int main()
{
    //freopen("in.cpp", "r", stdin);
    freopen("B-small-attempt2.in", "r", stdin);
    freopen("out.out", "w", stdout);
    int T, ncase = 0;
    scanf("%d", &T);
    while (T--)
    {
        scanf("%d", &n);
        for (int i = 1; i <= n; i++)scanf("%d", &a[i]);
        init();
        int ret = n * n * 100;
        for (int i = 0; i <= n; i++)
        {
            if (f[i][n - i] == -1)continue;
            pd(ret, f[i][n - i]);
            //printf("** %d: %d\n", i, f[i][n - i]);
        }
        printf("Case #%d: %d\n", ++ncase, ret / 2);
    }
    return 0;
}