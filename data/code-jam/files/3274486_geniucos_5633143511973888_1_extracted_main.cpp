#include<bits/stdc++.h>

using namespace std;

int N, K;
long double U, P[60], Q[60], dp[60][60];

long double getAns ()
{
    for (int i=0; i<=N; i++)
        for (int j=0; j<=N; j++)
            dp[i][j] = 0.0;
    dp[0][0] = 1.0;
    for (int i=1; i<=N; i++)
        for (int j=0; j<=i; j++)
        {
            dp[i][j] = (long double) dp[i - 1][j] * (1.0 - Q[i]);
            if (j > 0) dp[i][j] = (long double) dp[i][j] + dp[i - 1][j - 1] * Q[i];
        }
    long double ans = 0.0;
    for (int j=K; j<=N; j++)
        ans += dp[N][j];
    return ans;
}

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
    long double ans;
    for (int q = 1; q <= N; q ++)
    {
        long double p = 0.0, u = 1.0, mij, ras = P[N - q + 1];
        int steps = 400;
        while (steps --)
        {
            long double mij = (p + u) / 2.0, needed = 0.0;
            for (int i=N - q + 1; i<=N; i++)
                if (P[i] < mij)
                    needed += mij - P[i];
            if (needed <= U) ras = p = mij;
            else u = mij;
        }

        for (int i=1; i<=N; i++)
            Q[i] = P[i];
        for (int i=N - q + 1; i<=N; i++)
            if (P[i] < ras) Q[i] = ras;
        long double curr = getAns ();
        if (q == 1 || curr > ans) ans = curr;
    }
    g << setprecision (20) << ans << '\n';
}
return 0;
}
