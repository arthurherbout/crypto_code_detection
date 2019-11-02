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

int N, T, nr_tot;
pair < int, int > tot[109], a[109], tot2[109];

bool ok (pair < int, int > aa, pair < int, int > bb)
{
    int nr;
    nr = 2, a[1] = aa, a[2] = bb;
    sort (a + 1, a + nr + 1);
    if (a[1].first == a[2].first)
        return 1;

    if (a[1].second >= a[2].second)
    {
        long long D1, D2, D_r, D_t;
        D1 = a[1].second, D2 = a[2].second;
        D_r = 360 - (D1 - D2);
        D_t = 360 - D2;
        if (1LL * D_t * (a[2].first - a[1].first) < 1LL * D_r * a[1].first)
            return 1;
        return 0;
    }

    long long D1, D2, D_r, D_t;
    D1 = a[1].second, D2 = a[2].second;
    D_r = 360 + (D2 - D1);
    D_t = 360 - D2;
    if (1LL * D_t * (a[2].first - a[1].first) < 1LL * D_r * a[1].first)
        return 1;
    return 0;
}

int main()
{
freopen ("input", "r", stdin);
freopen ("output", "w", stdout);

int test = 0, T;
scanf ("%d", &T);
while (test < T)
{
    printf ("Case #%d: ", ++test);
    scanf ("%d", &N);
    nr_tot = 0;
    for (int i=1; i<=N; i++)
    {
        int aux1, aux2, aux3;
        scanf ("%d %d %d", &aux1, &aux2, &aux3);
        for (int j=1; j<=aux2; j++)
            tot[++nr_tot].first = aux3 + j - 1, tot[nr_tot].second = aux1;
    }
    if (nr_tot == 1) printf ("0\n");
    else
    {
        int maxi = 1;
        for (int msk = 1; msk < (1<<nr_tot); msk ++)
        {
            int nr2 = 0;
            for (int i=0; i<nr_tot; i++)
                if (msk & (1<<i))
                    tot2[++nr2] = tot[i+1];

            bool eok = 1;
            for (int i=1; i<=nr2; i++)
                for (int j=i+1; j<=nr2; j++)
                    if (ok (tot2[i], tot2[j]) == 0)
                        eok = 0;
            if (eok && nr2 > maxi)
                maxi = nr2;
        }
        printf ("%d\n", nr_tot - maxi);
    }
}

return 0;
}
