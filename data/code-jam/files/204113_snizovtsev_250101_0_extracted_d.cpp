#include <climits>
#include <cstring>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;

#define foreach(iter, cont) \
    for (typeof((cont).begin()) iter = (cont).begin(); iter != (cont).end(); iter++)
typedef long long LL;

double solve()
{
    int n;
    scanf("%d", &n);
    int x[n], y[n], r[n];
    for (int i = 0; i < n; i++)
        scanf("%d %d %d", &x[i], &y[i], &r[i]);

    if (n == 1)
        return r[0];
    if (n == 2)
        return max(r[0], r[1]);

    assert(n == 3);
    double a = hypot(x[1]-x[0], y[1]-y[0]);
    double b = hypot(x[2]-x[1], y[2]-y[1]);
    double c = hypot(x[2]-x[0], y[2]-y[0]);
    double v0 = max(r[0] + 0.0, (b + r[1] + r[2]) / 2.0);
    double v1 = max(r[1] + 0.0, (c + r[0] + r[2]) / 2.0);
    double v2 = max(r[2] + 0.0, (a + r[0] + r[1]) / 2.0);
    return min(min(v0, v1), v2);
}

int main()
{
    int T;
    scanf("%d", &T);
    for (int i = 1; i <= T; i++) {
        printf("Case #%d: %.6lf\n", i, solve());
    }
    return 0;
}
