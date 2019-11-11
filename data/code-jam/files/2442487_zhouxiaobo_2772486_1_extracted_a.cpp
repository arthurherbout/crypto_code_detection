#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <string>

using namespace std;

const int mo = 1000002013 * 2;

struct zxb
{
    int x;
    int num;
}z[2000];

int n, m;

bool cmp(const zxb &u, const zxb &v)
{
    if (u.x < v.x)
        return true;
    if (u.x > v.x)
        return false;
    return u.num > v.num;
}

int cal(int x)
{
    return (int)((((long long)(n)) + n - x + 1) * x % mo);
}

int main(void)
{
    freopen("A-large.in","r",stdin);
    freopen("A-large.out","w",stdout);
    int U, i, j, k, sum1, sum2;
    scanf("%d",&U);
    for(int T = 1; T <= U; ++T)
    {
        scanf("%d%d" , &n, &m);
        sum1 = sum2 = 0;
        for(i = 0; i < m; ++i)
        {
            int o, e, p;
            scanf("%d%d%d", &o, &e, &p);
            z[i].x = o;
            z[i].num = p;
            z[m + i].x = e;
            z[m + i].num = -p;
            sum1 = (int)((sum1 + cal(e - o) * ((long long)(p))) % mo);
        }
        sort(z, z + m + m, cmp);
        for(i = 0; i < m + m; ++i)
        {
            if (z[i].num >=0)
                continue;
            int p = -z[i].num;
            for(j = i - 1; j >= 0; --j)
            {
                if (p == 0)
                    break;
                if (z[j].num <= 0)
                    continue;
                k = min(p, z[j].num);
                p -= k;
                z[j].num -= k;
                sum2 = (int)((sum2 + cal(z[i].x - z[j].x) * ((long long)(k))) % mo);
            }
        }
        int ans = (int)((((long long)(sum1)) - sum2 + mo) % mo / 2);
        printf("Case #%d: %d\n", T, ans);
    }
    //system("PAUSE");
    return 0;
}
