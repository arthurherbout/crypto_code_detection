#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <string>

using namespace std;

int n;
long long p;

bool gg(int n, long long m, long long p)
{
    if (n == 1)
        return true;
    long long all = (1LL << n);
    if (m > 0)
    {
        long long pp = p - (all >> 1);
        if (pp <= 0)
            return false;
        return gg(n - 1, ((m - 1) >> 1), pp);
    }
    return true;
}

bool cc(int n, long long m, long long p)
{
    if (n == 1)
        return true;
    long long all = (1LL << n);
    if (m < all - 1)
    {
        return cc(n - 1, ((m + 1) >> 1), p);
    }
    return p >= all;
}

long long g(int n, long long p)
{
    long long l = 0;
    long long r = (1LL << n);
    while(l + 1 < r)
    {
        long long m = (l + r) / 2;
        if (gg(n, m, p))
            l = m;
        else r = m;
    }
    return l;
}

long long c(int n, long long p)
{
    long long l = 0;
    long long r = (1LL << n);
    while(l + 1 < r)
    {
        long long m = (l + r) / 2;
        if (cc(n, m, p))
            l = m;
        else r = m;
    }
    return l;
}

int main(void)
{
    freopen("B-small-attempt0.in","r",stdin);
    freopen("B-small-attempt0.out","w",stdout);
    int U;
    scanf("%d", &U);
    for(int T = 1; T <= U; ++T)
    {
        scanf("%d%I64d", &n, &p);
        printf("Case #%d: %I64d %I64d\n", T, g(n, p), c(n, p));
    }
    //system("PAUSE");
    return 0;
}
