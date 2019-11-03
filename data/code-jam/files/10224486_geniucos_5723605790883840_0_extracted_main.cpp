#include<bits/stdc++.h>

using namespace std;

int MAX, nr, N, M, ap[200], codX[20][20], codY[20][20], a[200], b[200];
char mat[20][20];
bool found;
vector < int > v[200];

void add_edge (int x, int y)
{
    v[x].push_back (y);
    v[y].push_back (x);
}

void dfs (int nod)
{
    ap[nod] = 1;
    for (auto it = v[nod].begin (); it != v[nod].end (); it ++)
        if (ap[*it] == 0) dfs (*it);
}

int main ()
{
freopen ("input", "r", stdin);
freopen ("answer.txt", "w", stdout);

int Tests;
scanf ("%d", &Tests);
for (int curr_test = 1; curr_test <= Tests; curr_test ++)
{
    printf ("Case #%d:\n", curr_test), found = 0;
    scanf ("%d %d", &N, &M), nr = 0;
    for (int i=1; i<=N + M; i++)
        scanf ("%d %d", &a[i], &b[i]);
    for (int j=1; j<=M; j++)
        codX[0][j - 1] = ++nr;
    for (int i=1; i<=N; i++)
        codY[i - 1][M] = ++nr;
    for (int j=M; j>=1; j--)
        codX[N][j - 1] = ++nr;
    for (int i=N; i>=1; i--)
        codY[i - 1][0] = ++nr;
    for (int j=1; j<M; j++)
        for (int i=0; i<N; i++)
            codY[i][j] = ++nr;
    for (int i=1; i<N; i++)
        for (int j=0; j<M; j++)
            codX[i][j] = ++nr;
    for (int msk = 0; msk < (1 << (N * M)); msk ++)
    {
        int curr_nr = 0;
        for (int i=1; i<=nr; i++)
            v[i].clear (), ap[i] = 0;
        for (int i=1; i<=N; i++)
            for (int j=1; j<=M; j++)
            {
                int sus, jos, stn, drp;
                sus = codX[i - 1][j - 1], stn = codY[i - 1][j - 1], drp = codY[i - 1][j], jos = codX[i][j - 1];
                if (msk & (1 << curr_nr)) add_edge (stn, jos), add_edge (sus, drp), mat[i][j] = 92;
                else add_edge (stn, sus), add_edge (drp, jos), mat[i][j] = '/';
                curr_nr ++;
            }
        bool ok = 1;
        for (int i=1; i<=N + M; i++)
        {
            if (ap[a[i]] || ap[b[i]])
            {
                ok = 0;
                break;
            }
            dfs (a[i]);
            if (!ap[b[i]])
            {
                ok = 0;
                break;
            }
        }
        if (ok)
        {
            found = 1;
            for (int i=1; i<=N; i++, printf ("\n"))
                for (int j=1; j<=M; j++)
                    printf ("%c", mat[i][j]);
            break;
        }
    }
    if (!found) printf ("IMPOSSIBLE\n");
}

return 0;
}
