#include <iostream>
#include <string>
#include <string.h>
#include <cstring>
#include <algorithm>
#include <map>
#include <set>
#include <vector>
#include <math.h>

using namespace std;

void solve(int test)
{
    int a1, a2, b1, b2;
    scanf("%d%d%d%d", &a1, &a2, &b1, &b2);
    long long res = (long long)(a2 - a1 + 1) * (long long)(b2 - b1 + 1);
    double p = (1.0 + sqrt(5.0)) / 2.0;
    for (int i = a1; i <= a2; i ++)
    {
        int s = ceil((double)(i) / p);
        int f = s + i - 1;
        int l = max(s, b1);
        int r = min(f, b2);

        if (l <= r) res -= (long long)(r - l + 1);
    }
    printf("Case #%d: %lld\n", test, res);
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