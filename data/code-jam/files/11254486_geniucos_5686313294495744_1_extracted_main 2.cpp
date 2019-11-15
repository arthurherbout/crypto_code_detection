#include<bits/stdc++.h>

using namespace std;

int N, M, Q, C[1009], st[1009], dr[1009];
char curr1[30], curr2[30];
map < string, int > mpa, mpb;
vector < int > v[1009];

bool pair_up (int nod)
{
    if (C[nod]) return 0;
    C[nod] = 1;
    for (vector < int > :: iterator it = v[nod].begin (); it != v[nod].end (); it ++)
        if (st[*it] == 0)
        {
            st[*it] = nod;
            dr[nod] = *it;
            return 1;
        }
    for (vector < int > :: iterator it = v[nod].begin (); it != v[nod].end (); it ++)
        if (pair_up (st[*it]))
        {
            st[*it] = nod;
            dr[nod] = *it;
            return 1;
        }
    return 0;
}

int max_cup ()
{
    for (int i=1; i<=N; i++)
        dr[i] = 0;
    for (int i=1; i<=M; i++)
        st[i] = 0;
    bool ok = 1;
    while (ok)
    {
        ok = 0;
        for (int i=1; i<=N; i++)
            C[i] = 0;
        for (int i=1; i<=N; i++)
            if (dr[i] == 0) ok |= pair_up (i);
    }
    int cnt = 0;
    for (int i=1; i<=N; i++)
        cnt += (dr[i] != 0);
    return cnt;
}

int main ()
{
freopen ("input", "r", stdin);
freopen ("output", "w", stdout);

int Tests;
scanf ("%d", &Tests);
for (int test_id = 1; test_id <= Tests; test_id ++)
{
    printf ("Case #%d: ", test_id);
    scanf ("%d", &Q), mpa.clear (), mpb.clear (), N = 0, M = 0;
    for (int k=1; k<=Q; k++)
    {
        scanf ("\n%s %s", curr1, curr2);
        string s1 = curr1, s2 = curr2;
        int i, j;
        if (mpa.count (s1)) i = mpa[s1];
        else mpa[s1] = i = ++ N;
        if (mpb.count (s2)) j = mpb[s2];
        else mpb[s2] = j = ++ M;
        v[i].push_back (j);
    }
    printf ("%d\n", Q - (N + M - max_cup ()));
    for (int i=1; i<=N; i++)
        v[i].clear ();
}

return 0;
}
