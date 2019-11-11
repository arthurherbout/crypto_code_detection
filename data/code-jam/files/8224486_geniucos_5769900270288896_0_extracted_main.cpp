#include<cstdio>
#include<vector>
#include<cstring>
#include<cmath>
#include<cstdlib>
#include<ctime>
#include<map>
#include<set>
#include<queue>
#include<algorithm>

using namespace std;

int nr, R, C, N, mini, ap[109][109], cod[109][109], X[109], Y[109];

int main()
{
//freopen ("input", "r", stdin);
//freopen ("output", "w", stdout);

int test = 0, T;
scanf ("%d", &T);
while (test < T)
{
    printf ("Case #%d: ", ++test);

    scanf ("%d %d %d", &R, &C, &N);
    nr = 0;
    for (int i=1; i<=R; i++)
        for (int j=1; j<=C; j++)
            cod[i][j] = nr ++, X[cod[i][j]] = i, Y[cod[i][j]] = j;

    int mini = 1000000;
    for (int msk = 0; msk < (1<<nr); msk ++)
    {
        for (int i=1; i<=R; i++)
            for (int j=1; j<=C; j++)
                ap[i][j] = 0;

        int vol = 0;
        for (int i=0; i<nr; i++)
            if (msk & (1<<i))
                ap[X[i]][Y[i]] = 1, vol ++;

        if (vol != N)
            continue;

        int Cost = 0;
        for (int i=1; i<R; i++)
            for (int j=1; j<=C; j++)
                Cost += (ap[i][j] & ap[i+1][j]);

        for (int i=1; i<=R; i++)
            for (int j=1; j<C; j++)
                Cost += (ap[i][j] & ap[i][j+1]);

        if (Cost < mini)
            mini = Cost;
    }
    printf ("%d\n", mini);
}

return 0;
}
