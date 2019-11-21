#include <iostream>
#include <string>
#include <string.h>
#include <cstring>
#include <vector>
#include <algorithm>
#include <set>
#include <map>

using namespace std;

int d[2000000];
long long T;
int n;
long long b[200];

void solve(int test)
{
    cerr << test << endl;
    cin >> T;
    scanf("%d", &n);
    for (int i = 1; i <= n; i ++)
        scanf("%d", &b[i]);

    sort(b + 1, b + n + 1);

    d[0] = 0;
    for (int i = 1; i <= 1000000; i ++)
    {
        d[i] = 10000000;
        for (int j = 1; j <= n; j ++)
            if (i >= b[j])
                d[i] = min(d[i], d[i - b[j]] + 1);
    }

    long long ans = 2000000000000000000LL;
    for (int i = 1; i <= 1000000; i ++)
    {
        if (d[i] == 10000000) continue;
        //for (int j = 1; j <= n; j ++)
        int j = n;
            if ((T - (long long)(i)) % (long long)(b[j]) == 0LL)
            {
                long long cur = (T - (long long)(i)) / (long long)(b[j]);
                cur += (long long)(d[i]);
                if (cur < ans) ans = cur;
            }
    }

    if (ans == 2000000000000000000LL)
        printf("Case #%d: IMPOSSIBLE\n", test); else
        printf("Case #%d: %lld\n", test, ans);

}

void solve2(int test)
{
    cin >> T;
    scanf("%d", &n);
    for (int i = 1; i <= n; i ++)
        scanf("%lld", &b[i]);

    sort(b + 1, b + n + 1);
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