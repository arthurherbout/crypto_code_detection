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

int N, T, nr;
pair < int, int > a[109];

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
    nr = 0;
    for (int i=1; i<=N; i++)
    {
        int aux1, aux2, aux3;
        scanf ("%d %d %d", &aux1, &aux2, &aux3);
        for (int j=1; j<=aux2; j++)
            a[++nr].first = aux3 + j - 1, a[nr].second = aux1;
    }
    if (nr == 1) printf ("0\n");
    else
    {
        sort (a + 1, a + nr + 1);
        if (a[1].first == a[2].first)
        {
            printf ("0\n");
            continue;
        }

        if (a[1].second >= a[2].second)
        {
            long long D1, D2, D_r, D_t;
            D1 = a[1].second, D2 = a[2].second;
            D_r = 360 - (D1 - D2);
            D_t = 360 - D2;
            if (1LL * D_t * (a[2].first - a[1].first) < 1LL * D_r * a[1].first)
                printf ("0\n");
            else printf ("1\n");
            continue;
        }

        long long D1, D2, D_r, D_t;
        D1 = a[1].second, D2 = a[2].second;
        D_r = 360 + (D2 - D1);
        D_t = 360 - D2;
        if (1LL * D_t * (a[2].first - a[1].first) < 1LL * D_r * a[1].first)
            printf ("0\n");
        else printf ("1\n");

 //       printf ("not yet\n");
    }
}

return 0;
}
