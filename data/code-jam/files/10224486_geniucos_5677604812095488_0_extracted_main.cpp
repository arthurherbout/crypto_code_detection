#include<bits/stdc++.h>

using namespace std;

int nr, N, K, a[100];
double maxi, P[100];

void back (int pos)
{
    if (pos == N + 1)
    {
        if (nr != K) return ;
        double P2 = 0.0;
        for (int msk = 0; msk < (1 << nr); msk ++)
        {
            double curr = 1.0;
            int curr2 = 0;
            for (int i=1; i<=nr; i++)
                if (msk & (1 << (i - 1))) curr = (double) curr * P[a[i]], curr2 ++;
                else curr = (double) curr * (1.0 - P[a[i]]);
            if (curr2 == K / 2) P2 += curr;
        }
        if (P2 > maxi)
            maxi = P2;
        return ;
    }
    back (pos + 1);
    if (nr < K) a[++nr] = pos, back (pos + 1), nr --;
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
    maxi = 0.0, back (1);
    printf ("%.10f\n", maxi);
}

return 0;
}
