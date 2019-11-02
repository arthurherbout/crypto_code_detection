#include<bits/stdc++.h>

using namespace std;

int N, K;
double maxi, P[209], p[209], dp[209][209];

double calc (int K)
{
    dp[0][0] = 1.0;
    for (int i=1; i<=K; i++)
        for (int j=0; j<=i; j++)
        {
            dp[i][j] = (double) dp[i - 1][j] * p[i];
            if (j > 0) dp[i][j] += (double) dp[i - 1][j - 1] * (1.0 - p[i]);
        }
    return dp[K][K / 2];
}

int main ()
{
freopen ("input", "r", stdin);
freopen ("answer.txt", "w", stdout);

int Tests;
scanf ("%d", &Tests);
for (int curr_test = 1; curr_test <= Tests; curr_test ++)
{
    printf ("Case #%d: ", curr_test);
    scanf ("%d %d", &N, &K);
    for (int i=1; i<=N; i++)
        scanf ("%lf", &P[i]);
    sort (P + 1, P + N + 1), maxi = 0;
    for (int i=0; i<=K; i++)
    {
        int nrnr = 0;
        for (int j=1; j<=i; j++)
            p[++nrnr] = P[j];
        for (int j=N; j>=N - (K - i) + 1; j--)
            p[++nrnr] = P[j];
        double curr = calc (K);
        if (curr > maxi) maxi = curr;
    }
    printf ("%.10f\n", maxi);
}

return 0;
}
