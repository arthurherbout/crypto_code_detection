#include<bits/stdc++.h>

using namespace std;

int R, P, S, nr, N;
char aux[100009], ans[100009], b[1009];
bool GATA;

bool solve (int N, int R, int P, int S)
{
    if (R + P + S == 1)
    {
        nr = 1;
        if (R) ans[1] = 'R';
        else
        if (P) ans[1] = 'P';
        else ans[1] = 'S';
        return 1;
    }
    int a, b, c;
    a = (R + P - S) / 2;
    b = (R + S - P) / 2;
    c = (P + S - R) / 2;
    if ((a + b + c) != (1 << (N - 1)) || a < 0 || b < 0 || c < 0) return 0;
    if (!solve (N - 1, b, a, c)) return 0;
    int nr_aux = nr;
    for (int i=1; i<=nr; i++)
        aux[i] = ans[i];
    nr_aux = nr, nr = 0;
    for (int i=1; i<=nr_aux; i++)
    {
        ans[++nr] = aux[i];
        if (aux[i] == 'R') ans[++nr] = 'S';
        else
        if (aux[i] == 'P') ans[++nr] = 'R';
        else ans[++nr] = 'P';
        if (ans[nr - 1] > ans[nr]) swap (ans[nr], ans[nr - 1]);
    }
    return 1;
}

char wn (char a, char b)
{
    if (a == b) return -1;
    if (a == 'R')
    {
        if (b == 'S') return a;
        return b;
    }
    if (a == 'S')
    {
        if (b == 'P') return a;
        return b;
    }
    ///a = P
    if (b == 'R') return a;
    return b;
}

bool mere ()
{
    char aux[100], aux2[100];
    int nr = 1 << N;
    for (int i=1; i<=nr; i++)
        aux[i] = b[i];
    for (int i=1; i<=N; i++)
    {
        for (int j=1; j<=nr / 2; j++)
        {
            aux2[j] = wn (aux[j * 2 - 1], aux[j * 2]);
            if (aux2[j] == -1) return 0;
        }
        nr /= 2;
        for (int j=1; j<=nr; j++)
            aux[j] = aux2[j];
    }
    return 1;
}

void back (int pos, int R, int P, int S)
{
    if (GATA) return ;
    if (pos == (1 << N) + 1)
    {
        if (mere ())
        {
            for (int i=1; i<=1 << N; i++)
                printf ("%c", b[i]);
            printf ("\n");
            GATA = 1;
        }
        return ;
    }
    if (P) b[pos] = 'P', back (pos + 1, R, P - 1, S);
    if (R) b[pos] = 'R', back (pos + 1, R - 1, P, S);
    if (S) b[pos] = 'S', back (pos + 1, R, P, S - 1);
}

void MakeSmall (int st, int dr)
{
    if (st == dr) return ;
    int mij = (st + dr) >> 1, i1 = st, i2 = mij + 1, l = mij - st + 1;
    MakeSmall (st, mij), MakeSmall (mij + 1, dr);
    while (l --)
    {
        if (ans[i1] != ans[i2])
        {
            if (ans[i1] < ans[i2]) return ;
            l = (dr - st + 1) / 2;
            for (int i=st; i<=mij; i++)
                swap (ans[i], ans[i + l]);
            break;
        }
        i1 ++, i2 ++;
    }
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
    scanf ("%d %d %d %d", &N, &R, &P, &S), GATA = 0;
    ///a = cnt of RP -> P
    ///b = cnt of RS -> R
    ///c = cnt of PS -> S
    ///a + b + c = (R + P + S) / 2 = 2 ^ (N - 1)
    ///a + b = R
    ///a + c = P
    ///b + c = S
    ///a = (R + P - S) / 2
    ///b = (R + S - P) / 2
    ///c = (P + S - R) / 2
    if (!solve (N, R, P, S)) printf ("IMPOSSIBLE\n");
    else
    {
        MakeSmall (1, nr);
        for (int i=1; i<=nr; i++)
            printf ("%c", ans[i]), b[i] = ans[i];
        printf ("\n");
    }
//    back (1, R, P, S);
  //  if (!GATA) printf ("IMPOSSIBLE\n");
}

return 0;
}
