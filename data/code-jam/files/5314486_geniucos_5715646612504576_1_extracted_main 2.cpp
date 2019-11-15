#include<bits/stdc++.h>

using namespace std;

int R, C, id[59][59];
char sir[59][59];
vector < pair < int, bool > > covered[59][59];

class SAT2
{
public:
    int N, nr, Q[20009], ap[20009], sol[20009];
    vector < int > v[20009], h[20009];

    int cod (int x)
    {
        if (x < 0) return N - x;
        return x;
    }

    int opus (int nod)
    {
        if (nod <= N) return nod + N;
        return nod - N;
    }

    void add_implication (int x, int y)
    {
        v[cod (x)].push_back (cod (y));
        h[cod (y)].push_back (cod (x));
    }

    void add_clause (int x, int y)
    {
        add_implication (-x, y);
        add_implication (-y, x);
//        printf ("%d %d\n", x, y);
    }

    void add_need (int x)
    {
        add_implication (-x, x);
//        printf ("%d\n", x);
    }

    void dfs (int nod)
    {
        ap[nod] = 1;
        for (vector < int > :: iterator it = v[nod].begin (); it != v[nod].end (); it ++)
            if (ap[*it] == 0) dfs (*it);
        Q[++nr] = nod;
    }

    void dfp (int nod)
    {
        if (sol[nod])
        {
            sol[0] = -1;
            return ;
        }
        sol[opus (nod)] = 1;
        ap[nod] = 0;
        for (vector < int > :: iterator it = h[nod].begin (); it != h[nod].end (); it ++)
            if (ap[*it] == 1) dfp (*it);
    }

    bool solve ()
    {
        for (int i=1; i<=2 * N; i++)
            if (ap[i] == 0) dfs (i);
        for (int i=nr; i>=1; i--)
            if (ap[Q[i]] == 1 && ap[opus (Q[i])] == 1) dfp (Q[i]);
        if (sol[0] == -1) return 0;
        return 1;
    }

    void Clear ()
    {
        for (int i=0; i<=2 * N; i++)
            sol[i] = ap[i] = Q[i] = 0, v[i].clear (), h[i].clear ();
        nr = N = 0;
    }
}sistem;

bool FAILED = 0, ap2[59][59], ap3[59][59][3][3], ok[5009][2], psbl;
void mark (int x, int y, int dx, int dy)
{
    if (ap3[x][y][dx + 1][dy + 1]) return ;
    ap3[x][y][dx + 1][dy + 1] = 1;
    if (sir[x][y] == '#' || FAILED) return ;
    if (sir[x][y] == '.')
    {
        ap2[x][y] = 1;
        mark (x + dx, y + dy, dx, dy);
        return ;
    }
    if (sir[x][y] == '|' || sir[x][y] == '-')
    {
        FAILED = 1;
        return ;
    }
    if (sir[x][y] == '/')
    {
        if (dx == 0)
        {
            if (dy == 1) mark (x - 1, y, -1, 0);
            else mark (x + 1, y, +1, 0);
        }
        else
        {
            if (dx == 1) mark (x, y - 1, 0, -1);
            else mark (x, y + 1, 0, +1);
        }
        return ;
    }
    if (sir[x][y] == 92)
    {
        if (dx == 0)
        {
            if (dy == -1) mark (x - 1, y, -1, 0);
            else mark (x + 1, y, +1, 0);
        }
        else
        {
            if (dx == -1) mark (x, y - 1, 0, -1);
            else mark (x, y + 1, 0, +1);
        }
        return ;
    }
}

bool check (int x, int y)
{
    memset (ap2, 0, sizeof (ap2));
    memset (ap3, 0, sizeof (ap3));
    int dx, dy; FAILED = 0;
    if (sir[x][y] == '-') dx = 0, dy = 1;
    else dx = 1, dy = 0;
    mark (x + dx, y + dy, dx, dy);
    if (FAILED) return 0;
    if (sir[x][y] == '-') dx = 0, dy = -1;
    else dx = -1, dy = 0;
    mark (x + dx, y + dy, dx, dy);
    if (FAILED) return 0;
    return 1;
}

int get_id_for (int node, int j)
{
    if (j) return node;
    return -node;
}

int main ()
{
freopen ("input", "r", stdin);
freopen ("output", "w", stdout);

int Tests;
scanf ("%d\n", &Tests);
for (int test_id = 1; test_id <= Tests; test_id ++)
{
    printf ("Case #%d: ", test_id);
    scanf ("%d %d\n", &R, &C), sistem.Clear ();
    for (int i=1; i<=R; i++)
        gets (sir[i] + 1);
    for (int i=0; i<=C + 1; i++)
        sir[0][i] = sir[R + 1][i] = '#';
    for (int i=0; i<=R + 1; i++)
        sir[i][0] = sir[i][C + 1] = '#';
    for (int i=1; i<=R; i++)
        for (int j=1; j<=C; j++)
            if (sir[i][j] == '-' || sir[i][j] == '|')
                id[i][j] = ++sistem.N;
    for (int i=1; i<=R; i++)
        for (int j=1; j<=C; j++)
            if (sir[i][j] == '-' || sir[i][j] == '|')
                for (int k=0; k<2; k++)
                {
                    if (k == 0) sir[i][j] = '-';
                    else sir[i][j] = '|';
                    if (!check (i, j))
                    {
                        ok[id[i][j]][k] = 0;
                        continue;
                    }
                    ok[id[i][j]][k] = 1;
                    for (int i2=1; i2<=R; i2++)
                        for (int j2=1; j2<=C; j2++)
                            if (ap2[i2][j2])
                                covered[i2][j2].push_back ({id[i][j], k});
                }
    psbl = 1;
    for (int i=1; i<=sistem.N; i++)
    {
        if (ok[i][0] == 0 && ok[i][1] == 0) psbl = 0;
        else
        if (ok[i][0] == 0) sistem.add_need (i);
        else
        if (ok[i][1] == 0) sistem.add_need (-i);
    }
    if (psbl)
    {
        for (int i=1; i<=R; i++)
            for (int j=1; j<=C; j++)
                if (sir[i][j] == '.')
                {
                    if (covered[i][j].empty ()) psbl = 0;
                    else
                    if (covered[i][j].size () == 1) sistem.add_need (get_id_for (covered[i][j][0].first, covered[i][j][0].second));
                    else
                    {
                        if (covered[i][j].size () > 2)
                        {
                            fprintf (stderr, "WTF %d %d\n", i, j);
                            exit (0);
                        }
                        sistem.add_clause (get_id_for (covered[i][j][0].first, covered[i][j][0].second),
                                           get_id_for (covered[i][j][1].first, covered[i][j][1].second));
                    }
                }
        if (psbl)
        {
            if (sistem.solve () == 0) psbl = 0;
        }
    }
    if (!psbl) printf ("IMPOSSIBLE\n");
    else
    {
/*        for (int i=1; i<=2 * sistem.N; i++)
            printf ("%d", sistem.sol[i]);
        printf ("\n");*/
        printf ("POSSIBLE\n");
        for (int i=1; i<=R; i++, printf ("\n"))
            for (int j=1; j<=C; j++)
            {
                if (sir[i][j] == '|' || sir[i][j] == '-')
                {
                    if (sistem.sol[id[i][j]]) sir[i][j] = '|';
                    else sir[i][j] = '-';
                }
                printf ("%c", sir[i][j]);
            }
    }
    ///clear
    for (int i=1; i<=R; i++)
        for (int j=1; j<=C; j++)
            id[i][j] = 0, covered[i][j].clear ();
}
return 0;
}
