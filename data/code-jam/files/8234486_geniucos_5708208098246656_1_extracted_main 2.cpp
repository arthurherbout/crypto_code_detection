#include<cstdio>
#include<cstring>
#include<string>
#include<map>
#include<vector>
#include<queue>
#include<deque>

using namespace std;

int T, N, M, down[109][109], up[109][109], left[109][109], right[109][109];
char sir[109][109];

int main ()
{
//freopen ("input", "r", stdin);
//freopen ("output", "w", stdout);

scanf ("%d", &T);
for (int test = 1; test <= T; test ++)
{
    printf ("Case #%d: ", test);

    scanf ("%d %d\n", &N, &M);
    for (int i=1; i<=N; i++)
        gets (sir[i] + 1);

    for (int i=1; i<=N; i++)
    {
        int j;
        for (j=1; j<=M; j++)
            if (sir[i][j] != '.')
                break;
        for (int p=1; p<=j; p++)
            left[i][p] = 0;
        for (int p=j+1; p<=M; p++)
            left[i][p] = j;
    }

    for (int i=1; i<=N; i++)
    {
        int j;
        for (j=M; j>=1; j--)
            if (sir[i][j] != '.')
                break;
        for (int p=j; p<=M; p++)
            right[i][p] = 0;
        for (int p=1; p<j; p++)
            right[i][p] = j;
    }

    for (int j=1; j<=M; j++)
    {
        int i;
        for (i=1; i<=N; i++)
            if (sir[i][j] != '.')
                break;
        for (int p=1; p<=i; p++)
            up[p][j] = 0;
        for (int p=i+1; p<=N; p++)
            up[p][j] = i;
    }

    for (int j=1; j<=M; j++)
    {
        int i;
        for (i=N; i>=1; i--)
            if (sir[i][j] != '.')
                break;
        for (int p=i; p<=N; p++)
            down[p][j] = 0;
        for (int p=1; p<i; p++)
            down[p][j] = i;
    }

    bool ok = 1;
    int sum = 0;
    for (int i=1; i<=N; i++)
        for (int j=1; j<=M; j++)
            if (sir[i][j] != '.')
            {
                if (up[i][j] == 0 && down[i][j] == 0 && right[i][j] == 0 && left[i][j] == 0)
                    ok = 0;
                else
                if (sir[i][j] == '<' && left[i][j] == 0)
                    sum ++;
                else
                if (sir[i][j] == '>' && right[i][j] == 0)
                    sum ++;
                else
                if (sir[i][j] == '^' && up[i][j] == 0)
                    sum ++;
                else
                if (sir[i][j] == 'v' && down[i][j] == 0)
                    sum ++;
            }

    if (ok == 0)
        printf ("IMPOSSIBLE\n");
    else
        printf ("%d\n", sum);

    for (int i=1; i<=N; i++)
        for (int j=1; j<=M; j++)
            up[i][j] = down[i][j] = left[i][j] = right[i][j] = 0;
}

return 0;
}
