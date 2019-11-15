#include<bits/stdc++.h>

using namespace std;

int ap[10], N, P, dp2[109], dp3[109][109], dp4[109][109][109];

void U (int &x, int y) {if (y > x) x = y;}

int main ()
{
freopen ("input", "r", stdin);
freopen ("output", "w", stdout);

N = 100;
for (int i=0; i<=N; i++)
{
    int curr = dp2[i] + (i % 2 == 0);
    U (dp2[i + 1], curr);
}
for (int i=0; i<=N; i++)
    for (int j=0; i + j<=N; j++)
    {
        int curr = dp3[i][j] + ((i + 2 * j) % 3 == 0);
        U (dp3[i + 1][j], curr);
        U (dp3[i][j + 1], curr);
    }
for (int i=0; i<=N; i++)
    for (int j=0; i + j<=N; j++)
        for (int k=0; i + j + k<=N; k++)
        {
            int curr = dp3[i][j] + ((i + 2 * j + 3 * k) % 4 == 0);
            U (dp4[i + 1][j][k], curr);
            U (dp4[i][j + 1][k], curr);
            U (dp4[i][j][k + 1], curr);
        }
int Tests;
scanf ("%d\n", &Tests);
for (int test_id = 1; test_id <= Tests; test_id ++)
{
    printf ("Case #%d: ", test_id);
    scanf ("%d %d", &N, &P);
    for (int i=0; i<P; i++)
        ap[i] = 0;
    for (int i=1; i<=N; i++)
    {
        int x;
        scanf ("%d", &x), ap[x % P] ++;
    }
    int ans = ap[0];
    if (P == 2) ans += dp2[ap[1]];
    else
    if (P == 3) ans += dp3[ap[1]][ap[2]];
    else ans += dp4[ap[1]][ap[2]][ap[3]];
    printf ("%d\n", ans);
}
return 0;
}
