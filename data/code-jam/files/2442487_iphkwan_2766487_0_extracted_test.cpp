#include <iostream>
#include <cstdio>
#include <string.h>
#include <algorithm>
#include <stack>
using namespace std;

int n;
long long p, a[60], b[60];
void cal()
{
    a[0] = 0;
    long long tmp = 2;
    int tot = 1;
    while(tot < n)
    {
        a[tot] = a[tot - 1] + tmp;
        tot++;
        tmp = tmp * 2;
    }
    a[n] = (1LL << n) - 1LL;
    b[0] = a[n];
    tot = 1;
    tmp = 1;
    while(tot < n)
    {
        b[tot] = b[tot - 1] - tmp;
        tot++;
        tmp = tmp * 2;
    }
    b[n] = 0;
    //cout << 1 << endl;
    //for (int i = 0; i <= n; i++)
        //printf("%lld %lld\n", a[i], b[i]);
}
long long solve1(int res, long long tmp, long long tp)
{
    //int res = 0;
    //long long tmp = (1LL << n) - 1LL;
    //long long tmp = (1LL << n) - 1LL;
    if (tp * 2 <= tmp)
        return a[res];
    return solve1(res + 1, tmp / 2, tp - (tmp / 2));
}
long long solve2(int res, long long tmp, long long tp)
{
    //int res = 0;
    //long long tmp = (1LL << n) - 1LL;
    //long long tp = p;
    if (tp >= tmp)
    {
        //cout << "res = "<<res<<endl;
        return b[res];
    }
    return solve2(res + 1, tmp / 2, tp);
}
int main()
{
    freopen("t.in", "r", stdin);
    freopen("t.txt", "w", stdout);
    int t;
    long long ans1, ans2;
    scanf("%d", &t);
    for (int z = 1; z <= t; z++)
    {
        scanf("%d%lld", &n, &p);
        cal();
        long long tmp = (1LL << n);
        //cout << "tmp ="<<tmp <<endl;
        long long tp = p;
        //cout << 1 << endl;
        if (tmp == p)
            ans1 = p - 1;
        else
            ans1 = solve1(0, tmp, tp);
        //cout << 1 << endl;
        ans2 = solve2(0, tmp, tp);
        //cout << 1 << endl;
        printf("Case #%d: %lld %lld\n", z, ans1, ans2);
    }
    return 0;
}
