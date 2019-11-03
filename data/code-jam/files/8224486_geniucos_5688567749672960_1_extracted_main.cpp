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

int cif[20], dp[1000009], how[1000009];
long long N, P[20];
queue < int > cc;

int rev (int x)
{
    int sol = 0;
    while (x)
        sol = sol * 10 + x % 10, x /= 10;
    return sol;
}

long long rev (long long x)
{
    long long sol = 0;
    while (x)
        sol = (long long) 1LL * sol * 10 + x % 10, x /= 10;
    return sol;
}

int nr_cif (int N)
{
    int nr = 0;
    while (N)
        nr ++, N /= 10;
    return nr;
}

long long get_sol (long long N)
{
    if (N <= 11)
        return N;

    int nr = 0;
    long long aux = N;
    while (aux)
        cif[++nr] = aux % 10, aux /= 10;
    reverse (cif + 1, cif + nr + 1);

    int jum = nr >> 1;

    if (nr % 2 == 0)
    {
        bool ok = 1;

        if (cif[1] != 1)
            ok = 0;

        for (int i=2; i<=jum; i++)
            if (cif[i] != 0)
                ok = 0;

        if (ok)
            return N - P[nr-1] + get_sol (P[nr-1] - 1) + 1;

        aux = 0;
        for (int i=1; i<=jum; i++)
            aux = 1LL * aux * 10 + cif[i];

        long long sol;
        if (1LL * aux * P[jum] + 1 <= N) sol =  N - 1LL * aux * P[jum] + get_sol (rev (1LL * aux * P[jum] + 1));
        else aux --, sol = N - 1LL * aux * P[jum] + get_sol (rev (1LL * aux * P[jum] + 1));
        if (rev (1LL * aux * P[jum] + 1) == 1LL * aux * P[jum] + 1)
            sol --;
        return sol;
    }
    bool ok = 1;

    if (cif[1] != 1)
        ok = 0;

    for (int i=2; i<=jum; i++)
        if (cif[i] != 0)
            ok = 0;

    if (ok)
        return N - P[nr-1] + get_sol (P[nr-1] - 1) + 1;

    aux = 0;
    for (int i=1; i<=jum + 1; i++)
        aux = 1LL * aux * 10 + cif[i];

    long long sol;
    if (1LL * aux * P[jum] + 1 <= N) sol =  N - 1LL * aux * P[jum] + get_sol (rev (1LL * aux * P[jum] + 1));
    else aux --, sol = N - 1LL * aux * P[jum] + get_sol (rev (1LL * aux * P[jum] + 1));
    if (rev (1LL * aux * P[jum] + 1) == 1LL * aux * P[jum] + 1)
        sol --;
    return sol;
}

int main()
{
freopen ("input", "r", stdin);
freopen ("output", "w", stdout);

P[0] = 1;
for (int i=1; i<=16; i++)
    P[i] = 1LL * P[i-1] * 10;

/*get_sol (21);
return 0;*/

/*cc.push (1);
dp[1] = 1;
while (!cc.empty ())
{
    int nod = cc.front ();
    cc.pop ();
    if (dp[nod + 1] == 0 && nod + 1 <= 1000000)
    {
        dp[nod + 1] = dp[nod] + 1;
        how[nod + 1] = nod;
        cc.push (nod + 1);
    }
    if (dp[rev (nod)] == 0 && rev (nod) <= 1000000)
    {
        dp[rev(nod)] = dp[nod] + 1;
        how[rev(nod)] = nod;
        cc.push (rev (nod));
    }
}*/

/*get_sol (20);
return 0;*/

/*for (int i=1; i<=10000; i++)
    if (how[i] != i - 1)
        printf ("%5d -> %5d\n", i, how[i]);
return 0;*/

/*int maxi = 0;
for (int i=1; i<=1000000; i++)
    if (dp[i] > maxi)
        maxi = dp[i];
printf ("%d\n", maxi);
return 0;*/

/*int wr = 0;
for (int i=1; i<=1000000; i++)
    if (get_sol (i) != dp[i])
    {
        printf ("%d\n", i);
        if (++wr == 1000)
            return 0;
    }*/

int test = 0, T;
scanf ("%d", &T);
while (test < T)
{
    printf ("Case #%d: ", ++test);
    scanf ("%lld", &N);
    printf ("%lld\n", get_sol (N));
}

return 0;
}
