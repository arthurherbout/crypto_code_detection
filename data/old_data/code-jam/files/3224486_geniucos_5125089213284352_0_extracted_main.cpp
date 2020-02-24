#include<bits/stdc++.h>

using namespace std;

int Q, L, N, step, t[109], phi[109], ap[109], vol[109], nxt[109][509];
bool Fin;
double good, bad;
char sir[109], curr[109];
clock_t t1;
vector < int > v[109];

int Nxt (int i, char c)
{
    while (i && curr[i + 1] != c) i = phi[i];
    if (curr[i + 1] == c) i ++;
    return i;
}

bool cmp (int i, int j)
{
    return (vol[i] > vol[j]);
}

void Prec ()
{
    for (int i=0; i<=L; i++)
        for (int j='A'; j<='Z'; j++)
            nxt[i][j] = Nxt (i, j);
}

void RBack (int state, double p, int sz)
{
    if (Fin) return ;
    if (++step == 350000)
    {
        /*sclock_t t2 = clock ();
        if ((t2 - t1) / CLOCKS_PER_SEC > 0.2)
        {
            Fin = 1;
            return ;
        }*/
        Fin = 1;
        step = 0;
    }
    if (state == L)
    {
        good += p, bad += p;
        return ;
    }
    vector < int > poate;
    for (int i=1; i<=N; i++)
        if (ap[i] == 0 && ap[t[i]]) poate.push_back (i);
    if (poate.empty ())
    {
        bad += p;
        return ;
    }
    random_shuffle (poate.begin (), poate.end ());
    for (auto it = poate.begin (); it != poate.end (); it ++)
        ap[*it] = 1, RBack (nxt [state][sir[*it]], (double) p * vol[*it] / sz, sz - 1), ap[*it] = 0;
}

void df (int nod)
{
    vol[nod] = 1;
    for (auto it = v[nod].begin (); it != v[nod].end (); it ++)
        df (*it), vol[nod] += vol[*it];
}

int main ()
{
freopen ("input", "r", stdin);
freopen ("answer.txt", "w", stdout);

srand (time (0));
/*for (int i=1; i<=100; i++)
    printf ("%d ", i / 2);
printf ("\n");
for (int i=1; i<=100; i++)
    printf ("A");
printf ("\n");
printf ("1\nAAAAAAAAAAAAAAAA");
return 0;*/

int Tests;
scanf ("%d", &Tests);
for (int curr_test = 1; curr_test <= Tests; curr_test ++)
{
    printf ("Case #%d:", curr_test);
    scanf ("%d\n", &N);
    for (int i=1; i<=N; i++)
        scanf ("%d", &t[i]), v[t[i]].push_back (i);
    df (0);
    scanf ("\n");
    gets (sir + 1);
    scanf ("%d", &Q), ap[0] = 1;
    while (Q --)
    {
        char ceva;
        scanf ("%c%s", &ceva, curr + 1), L = strlen (curr + 1);
        int k = 0;
        for (int i=2; i<=L; i++)
        {
            while (k && curr[k + 1] != curr[i]) k = phi[k];
            if (curr[k + 1] == curr[i]) k ++;
            phi[i] = k;
        }
        Prec (), step = 0, Fin = 0, t1 = clock (), bad = 0.0, good = 0.0, RBack (0, 1.0, N);
        printf (" %.10f", (double) good / bad);
    }
    printf ("\n");
    for (int i=0; i<=N; i++)
        v[i].clear ();
}

return 0;
}
