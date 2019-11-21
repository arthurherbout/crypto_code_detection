#include <iostream>
#include <string>
#include <string.h>
#include <cstring>
#include <vector>
#include <algorithm>
#include <set>
#include <map>

using namespace std;

int p[2000], m;
int p10[20];

int a[20];
bool used[20000];

void solve(int test)
{
    int d, k;
    scanf("%d%d", &d, &k);
    for (int i = 1; i <= k; i ++)
        scanf("%d", &a[i]);

    for (int i = 0; i <= 10000; i ++)
        used[i] = false;

    int mx = 0;
    for (int i = 1; i <= k; i ++)
        mx = max(mx, a[i]);

    if (k == 1)
    {
        printf("Case #%d: I don't know.\n", test);
        return;
    }

    for (int i = 1; i <= m && p[i] <= p10[d]; i ++)
    {
        if (p[i] <= mx) continue;
        int P = p[i], A, B, S;
        for (int j = 0; j < P; j ++)
        {
            A = j;
            S = a[1];

            B = (a[2] - A * S) % P;
            B = (B + P) % P;

            bool good = true;
            S = a[2];
            for (int i = 3; i <= k; i ++)
            {
                S = (S * A + B) % P;
                if (S != a[i]) 
                {
                    good = false;
                    break;
                }
            }

            if (!good) continue;

            int res = (a[k] * A + B) % P;
            used[res] = true;
        }
    }

    int get = 0;
    for (int i = 0; i <= 10000; i ++)
        if (used[i]) get ++;
    if (get > 1)
    {
        printf("Case #%d: I don't know.\n", test);
        return;
    } else
    {
        for (int i = 0; i <= 10000; i ++)
            if (used[i]) 
            {
                printf("Case #%d: %d\n", test, i);
                return;
            }
    }
}

bool is_prime(int n)
{
    for (int i = 2; i * i <= n; i ++)
        if (n % i == 0)
            return false;
    return true;
}

int main()
{
    freopen("input.txt","r",stdin);
    freopen("output.txt","w",stdout);
    int test;
    scanf("%d", &test);

    for (int i = 2; i <= 10000; i ++)
        if (is_prime(i)) m ++, p[m] = i;
    p10[0] = 1;
    for (int i = 1; i <= 9; i ++)
        p10[i] = p10[i - 1] * 10;

    for (int i = 1; i <= test; i ++)
        solve(i);
    return 0;
}