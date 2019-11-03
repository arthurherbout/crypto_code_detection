#include<bits/stdc++.h>

using namespace std;

int nr, best, N, p[5], x[20], y[20], ap[5];
bool GATA;
char sir[5][5];

void back (int pos)
{
    if (GATA) return ;
    if (pos == N + 1) return ;
    bool least_one_move = 0;
    for (int j=1; j<=N; j++)
        if (ap[j] == 0 && sir[p[pos]][j])
            least_one_move = 1;
    if (!least_one_move)
    {
        GATA = 1;
        return ;
    }
    for (int j=1; j<=N; j++)
        if (ap[j] == 0 && sir[p[pos]][j])
            ap[j] = 1, back (pos + 1), ap[j] = 0;
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
    scanf ("%d\n", &N), nr = 0, best = 20;
    for (int i=1; i<=N; i++)
        gets (sir[i] + 1);
    for (int i=1; i<=N; i++)
        for (int j=1; j<=N; j++)
        {
            sir[i][j] -= '0';
            if (sir[i][j] == 0) x[++nr] = i, y[nr] = j;
        }
    for (int msk = 0; msk < (1 << nr); msk ++)
    {
        int curr_cost = 0;
        for (int i=0; i<nr; i++)
            if (msk & (1 << i)) curr_cost ++, sir[x[i + 1]][y[i + 1]] = 1;
        for (int i=1; i<=N; i++)
            p[i] = i;
        bool ok = 1;
        do {
            GATA = 0, back (1);
            if (GATA)
            {
                ok = 0;
                break;
            }
        }while (next_permutation (p + 1, p + N + 1));
        if (ok && curr_cost < best)
            best = curr_cost;
        for (int i=0; i<nr; i++)
            if (msk & (1 << i)) sir[x[i + 1]][y[i + 1]] = 0;
    }
    printf ("%d\n", best);
}

return 0;
}
