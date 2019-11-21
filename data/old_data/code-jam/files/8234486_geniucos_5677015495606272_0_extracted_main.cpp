#include<cstdio>
#include<cstring>
#include<string>
#include<map>
#include<vector>
#include<queue>
#include<deque>

using namespace std;

int c[10], sol2, N, M, nr, T, cod[48000][10], val[10], msk[48000], need[10], dp[10][1800], dap[1800], A[10][10];
int ans[10][10];

int mincirc ()
{
    int ok = 1;
    for (int i=0; i<M; i++)
    {
        int nr = 0;
        for (int j=i+1; j <=M; j++)
            c[++nr] = val[j];
        for (int j=1; j <=i; j++)
            c[++nr] = val[j];
        int k;
        for (k=1; k<=M; k++)
            if (c[k] != val[k])
            {
                if (c[k] < val[k])
                    ok = 0;
                break;
            }
    }
    return ok;
}

void back (int pos)
{
    if (pos == M + 1)
    {
        val[0] = val[M], val[M+1] = val[1];
        for (int i=1; i<=M; i++)
            if (val[i] == 3 && val[i-1] != 3 && val[i+1] != 3)
                return;
        for (int i=1; i<=M; i++)
            if (val[i] == 1 && val[i-1] == 1 && val[i+1] != 1)
                return;
        int iep = mincirc ();
        for (int Nsk = 0; Nsk < (1<<M); Nsk ++)
        {
            //if (Nsk == 10)
              //  Nsk = 10;
            for (int i=1; i<=M; i++)
                need[i] = ((Nsk & (1<<(i-1))) > 0);
            bool ok = 1, ok2 = 1;
            for (int i=1; i<=M; i++)
            {
                if (need[i] + (val[i-1] == val[i]) + (val[i+1] == val[i]) > val[i])
                    ok = 0;
                if (need[i] + (val[i-1] == val[i]) + (val[i+1] == val[i]) + 1 < val[i])
                    ok = 0;
                if (need[i] + (val[i-1] == val[i]) + (val[i+1] == val[i]) != val[i])
                    ok2 = 0;
            }
            if (ok)
            {
                nr ++;
                dap[nr] = iep;
                cod[nr][0] = val[M];
                cod[nr][M+1] = val[1];
                for (int i=1; i<=M; i++)
                    cod[nr][i] = val[i];
                msk[nr] = Nsk;
                dp[1][nr] = 0;

                /*printf ("%d:\n", nr);
                for (int i=1; i<=M; i++)
                    printf ("%d", val[i]);
                printf ("\n");
                for (int i=1; i<=M; i++)
                    printf ("%d", ((Nsk & (1<<(i-1))) > 0));
                printf ("\n\n");*/

            }
            if (ok2)
                dp[1][nr] = 1;
        }
        return ;
    }
    for (int j=1; j<=3; j++)
        val[pos] = j, back (pos + 1);
}

bool ok (int i, int j)
{
     int ce = 0;
    ce += (A[i][j] == A[i-1][j]);
    ce += (A[i][j] == A[i+1][j]);
    ce += (A[i][j] == A[i][j+1]);
    ce += (A[i][j] == A[i][j-1]);
    return (ce  == A[i][j]);
}

void back2 (int X, int Y)
{
    if (Y == 1 && X != 1)
    {
        A[X][0] = A[X][M], A[X][M+1] = A[X][1];
        for (int i=1; i<=M; i++)
            if (!ok (X, i))
                return;
    }
    if (X == N + 1)
    {
        for (int i=1; i<=N; i++)
            A[i][0] = A[i][M], A[i][M+1] = A[i][1];
/*        bool ok = 1;
        if (A[1][1] == 1 && A[1][2] == 2 && A[1][3] == 2 && A[2][1] == 1 && A[2][2] == 2 && A[2][3] == 2)
            A[1][1] = 1;
        if (ok)
        {*/
            for (int i=1; i<=N; i++, printf ("\n"))
                for (int j=1; j<=M; j++)
                    printf ("%d", A[i][j]);
            printf ("\n");
            sol2 ++;
        //}
        return;
    }
    int nx = X, ny = Y;
    if (ny == M)
        ny = 1, nx ++;
    else
        ny ++;
    for (int i=1; i<=3; i++)
        A[X][Y] = i, back2 (nx, ny);
}

int main ()
{
freopen ("input", "r", stdin);
freopen ("output", "w", stdout);

scanf ("%d", &T);
int mod = 1000000007;
for (int test = 1; test <= T; test ++)
{
    printf ("Case #%d: ", test);
    scanf ("%d %d", &N, &M);
    nr = 0;
    back (1);
    for (int i=1; i<N; i++)
        for (int j=1; j<=nr; j++)
            if (dp[i][j])
            {
                for (int k=1; k<=nr; k++)
                {
                    bool ok = 1;
                    for (int p=1; p<=M; p++)
                        if ( ((msk[j] & (1<<(p-1)))>0) != (cod[k][p] == cod[j][p]))
                            ok = 0;
                    if (ok == 0)
                        continue;
                    for (int p=1; p<=M; p++)
                        if ( ((msk[j] & (1<<(p-1)))>0) + ((msk[k] & (1<<(p-1)))>0) + (cod[k][p] == cod[k][p-1]) + (cod[k][p] == cod[k][p+1]) != cod[k][p])
                            ok = 0;
                    if (ok)
                    {
                        dp[i+1][k] += dp[i][j];
                        if (dp[i+1][k] >= mod)
                            dp[i+1][k] -= mod;
                    }
                }
            }

    int sol = 0;
    sol2 = 0;
    for (int j=1; j<=nr; j++)
    if (msk[j] == 0)
    {
        if (dp[N][j] > 0)
            dp[N][j] = 1;
        sol += dp[N][j];
        //printf ("si %d cu %d\n", j, dp[N][j]);
        if (sol >= mod)
            sol -= mod;
    }
    printf ("%d\n", sol);
    /*back2 (1, 1);
    printf ("%d fatza de %d\n", sol, sol2);*/
    for (int i=1; i<=N; i++)
        for (int j=1; j<=nr; j++)
            dp[i][j] = 0;
}

return 0;
}
