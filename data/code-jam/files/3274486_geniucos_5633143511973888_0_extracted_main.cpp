#include<bits/stdc++.h>

using namespace std;

int N, K;
long double U, P[60];

int main ()
{
ifstream f ("input");
ofstream g ("output");

int Tests;
f >> Tests;
for (int test_id = 1; test_id <= Tests; test_id ++)
{
    g << "Case #" << test_id << ": ";
    f >> N >> K;
    f >> U;
    for (int i=1; i<=N; i++)
        f >> P[i];
    sort (P + 1, P + N + 1);
    long double p = 0.0, u = 1.0, mij, ras = P[1];
    int steps = 400;
    while (steps --)
    {
        long double mij = (p + u) / 2.0, needed = 0.0;
        for (int i=1; i<=N; i++)
            if (P[i] < mij)
                needed += mij - P[i];
        if (needed <= U) ras = p = mij;
        else u = mij;
    }
    long double ans = 1.0;
    for (int i=1; i<=N; i++)
        if (P[i] > ras) ans = ans * P[i];
        else ans = ans * ras;
    g << setprecision (20) << ans << '\n';
}
return 0;
}
