#include <cstdio>
#include <cstdlib>
#include <iostream>

using namespace std;

char w[521196][30];
char st[5000];

int f[5000][5];

int main(void)
{
    FILE *fin = fopen("dictionary.txt", "r");    
    int i, j, n, k, maxlen = 0;
    for(i = 0; i < 521196; ++i)
    {
        fscanf(fin, "%s", w[i]);
       // maxlen = max(maxlen, (int)strlen(w[i]));
    }
    //cout << maxlen << endl;
    fclose(fin);
    freopen("C-small-attempt1.in","r",stdin);
    freopen("C-small-attempt1.out","w",stdout);
    int U;
    scanf("%d",&U);
    for(int T = 1; T <= U; ++T)
    {
        scanf("%s", st);
        n = strlen(st);
        for(i = 0; i <= n; ++i)
            for(j = 0; j <= 4; ++j)
                f[i][j] = 10000;
        f[0][0] = 0;
        for(i = 0; i < n; ++i)
            for(j = 0; j <= 4; ++j)
            {
                if (f[i][j] >= 10000)
                    continue;
                for(k = 0; k < 521196; ++k)
                {
                    char *p = w[k];
                    int jj = j, res = 0, m = strlen(p);
                    if (i + m > n)
                        continue;
                    int kk;
                    for(kk = 0; kk < m; ++kk)
                    {
                        if (p[kk] != st[i + kk])
                        {
                            if (jj == 0)
                                jj = 4;
                            else break;
                            ++res;
                        }
                        else jj = max(jj - 1, 0);
                    }
                    if (kk == m)
                    {
                        f[i + m][jj] = min(f[i + m][jj], f[i][j] + res);
                    }
                }
            }
        int ans = 10000;
        for(j = 0; j <= 4; ++j)
            ans = min(ans, f[n][j]);
        printf("Case #%d: %d\n", T, ans);
    }
    //system("PAUSE");
    return 0;
}
