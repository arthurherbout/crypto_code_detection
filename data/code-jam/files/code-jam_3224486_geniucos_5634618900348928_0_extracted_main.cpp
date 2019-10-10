#include<bits/stdc++.h>

using namespace std;

int INF = 1000000, N, dp[59][59];
char sir[59];

int main ()
{
freopen ("input", "r", stdin);
freopen ("answer.txt", "w", stdout);

int Tests;
scanf ("%d\n", &Tests);
for (int curr_test = 1; curr_test <= Tests; curr_test ++)
{
    printf ("Case #%d: ", curr_test);
    gets (sir + 1), N = strlen (sir + 1);
    for (int i=0; i<=N + 1; i++)
        for (int j=0; j<=N + 1; j++)
            dp[i][j] = -INF;
    for (int i=0; i<=N; i++)
        dp[i + 1][i] = 0;
    for (int i=N; i>=1; i--)
        for (int j=i + 1; j<=N; j++)
        {
            for (int k=i; k<=j; k++)
                if (dp[i][k] + dp[k + 1][j] > dp[i][j])
                    dp[i][j] = dp[i][k] + dp[k + 1][j];
            int curr = 5 + dp[i + 1][j - 1];
            if (sir[i] == sir[j]) curr += 5;
            if (dp[i][j] < curr) dp[i][j] = curr;
        }
/*    int maxi = 0;
    for (int i=1; i<=N; i++)
        if (dp[1][i] > maxi) maxi = dp[1][i];*/
    printf ("%d\n", dp[1][N]);
}

return 0;
}
