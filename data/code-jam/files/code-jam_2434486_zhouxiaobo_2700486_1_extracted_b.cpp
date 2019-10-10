#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <map>
#include <set>

using namespace std;

bool b[101][21];
int res[21][101][21];
int o = 50;
int qx[21], qy[21];

int main(void)
{
    freopen("B-small-attempt0.in","r",stdin);
    freopen("B-small-attempt0.out","w",stdout);
    int U;
    int n, x, y, i, t;
    scanf("%d",&U);
    for(t = 0; t < (1 << 20); ++t)
    {
        for(i = 1; i <= 20; ++i)
        {
            x = 0;
            y = 20;
            while(true)
            {
                if (y == 0)
                    break;
                if (b[o + x - 1][y - 1] && b[o + x + 1][y - 1])
                    break;
                if (b[o + x - 1][y - 1])
                {
                    ++x;
                    --y;
                    continue;
                }
                if (b[o + x + 1][y - 1])
                {
                    --x;
                    --y;
                    continue;
                }
                if (y > 1 && b[o + x][y - 2])
                {
                    if ((t >> (i - 1)) & 1)
                    {
                        ++x;
                        --y;
                    }
                    else
                    {
                        --x;
                        --y;
                    }
                }
                else
                    --y;
            }
            b[o + x][y] = true;
            qx[i] = o + x;
            qy[i] = y;
            ++res[i][o + x][y];
        }
        for(i = 1; i <= 20; ++i)
            b[qx[i]][qy[i]] = false;
    }

    for(int T = 1; T <= U; ++T)
    {
        scanf("%d%d%d", &n, &x, &y);
        int ans = 0;
        if (y > 20) y = 20;
        if (x < -50) x = -50;
        if (x > 50) x = 50;
        for(i = 1; i <= n; ++i)
            ans += res[i][o + x][y];
        printf("Case #%d: %.9lf\n", T, (ans + 0.0) / (1 << 20));
    }
    return 0;
}
