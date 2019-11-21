#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <set>
#include <map>
#include <cstdio>
#include <cstdlib>
using namespace std;

#define LL long long

const int T = 10000000;

int solve(LL A, LL B, LL n, LL v, LL y0, LL x0, LL vy, LL vx)
{
    if (vx == 0 || vy == 0)
        return -1;
    if (vx < 0) {
        x0 = -x0;
        vx = -vx;
    }
    if (vy < 0) {
        y0 = -y0;
        vy = -vy;
    }
    LL bound = A * vx, mo = bound * 2, dat = vy * 2 * B;
    LL f0 = y0 * vx - vy * (x0 + B);
    f0 = f0 % mo;
    if (f0 < 0)
        f0 += mo;
    LL fn = f0;
    for (int i = 1; i <= n; ++i)
        fn = (fn + dat) % mo;
    double t = 2;
    t *= B;
    t *= v;
    t *= n;
    if (t > bound)
        return -1;
    LL maxdis = 2 * B * v * n;
    for (int i = 1; i <= T; ++i) {
        f0 = (f0 + dat) % mo;
        fn = (fn + dat) % mo;
        LL dis;
        if ((f0 < bound && fn > bound) || (f0 > bound && fn < bound))
            dis = abs(mo - f0 - fn);
        else
            dis = abs(f0 - fn);
        if (dis > maxdis)
            return i - 1 + n;
    }
    return -1;
}

void work()
{
    LL A, B, n, m, vn, vm, x0, y0, vx, vy;
    cin >> A >> B;
    cin >> n >> m;
    cin >> vn >> vm;
    cin >> y0 >> x0 >> vy >> vx;
    int tn = solve(A, B, n, vn, y0, B - x0, vy, -vx);
    int tm = solve(A, B, m, vm, y0, x0, vy, vx);
    if (tn == -1 && tm == -1)
        cout << " DRAW";
    else
        if (tn == -1 || (tm != -1 && (tm < tn || tm == tn && vx > 0)))
            cout << " LEFT " << tm;
        else
            cout << " RIGHT " << tn;
}

int main()
{
    freopen("d1.in", "r", stdin);
    freopen("d1.out", "w", stdout);

    int t2;
    cin >> t2;
    for (int t1 = 1; t1 <= t2; ++t1) {
        printf("Case #%d:", t1);
        work();
        printf("\n");
    }
    
    return 0;
}
