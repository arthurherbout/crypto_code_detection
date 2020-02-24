#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <string>
#include <cmath>
#include <cstring>

using namespace std;

typedef long long ll;
typedef double db;

#define forab(i, a, b) for(int i = int(a); i < int(b); ++i)
#define forba(i, b, a) for(int i = int(b) - 1; i >= int(a); --i)
#define forn(i, n) forab(i, 0, n)

const int di[] = { 1, 1, 0,-1,-1, 0};
const int dj[] = { 0, 1, 1, 0,-1,-1};

int T;

struct pr {
    int i, j;

    pr() {i = j = 0;}
    pr(int _i, int _j) {i = _i, j = _j;}

    bool operator ==(const pr & x) const {
        return ((i == x.i) && (j == x.j));
    }

    bool operator <(const pr & x) const {
        if (i != x.i) return i < x.i;
        return j < x.j;
    }
};

bool z[6010][6010];
pr c[6010][6010];
int k, k2;

int s, m;
int x[10010], y[10010];

pr e[20000];
int sz;

int ans1, ans2, ans3, ansmn, ans;

pr get(pr v) {
    if (v.i == -1) return v;
    if (c[v.i][v.j].i == v.i && c[v.i][v.j].j == v.j) return c[v.i][v.j];
    return c[v.i][v.j] = get(c[v.i][v.j]);
}

bool set(pr a, pr b) {
    pr aa = get(a), bb = get(b);
    if (aa == bb) return 0;
    if (rand() & 1) c[aa.i][aa.j] = bb;
    else c[bb.i][bb.j] = aa;
    return 1;
}

void upd(int i, int j) {
    z[i][j] = 1;
    c[i][j] = pr(i, j);
    forn(w, 6)
        if (z[i + di[w]][j + dj[w]]) set(pr(i, j), pr(i + di[w], j + dj[w]));
}

void obhod() {
    int i = 1, j = 1;
    forn(w, 6)
        forn(q, s - 1) {
            if (q > 0) e[sz++] = get(pr(i, j));
            i += di[w], j += dj[w];
        }
}

void dfs(int i, int j) {
    forn(w, 6)
        if ((!z[i + di[w]][j + dj[w]]) && c[i + di[w]][j + dj[w]].i == -1) {
            c[i + di[w]][j + dj[w]] = c[i][j];
            dfs(i + di[w], j + dj[w]);
        }
}

void upd2(int i, int j) {
    z[i][j] = 0;
    c[i][j] = pr(i, j);
    k++;
    forn(w, 6)
        if (!z[i + di[w]][j + dj[w]])
            if (set(pr(i, j), pr(i + di[w], j + dj[w]))) k--;
}


int main()
{
    freopen("input.txt","r",stdin);
    freopen("output.txt","w",stdout);
    srand(time(NULL));
    scanf("%d", &T);
    forn(t, T) {
        scanf("%d%d", &s, &m);
        forn(i, m) scanf("%d%d", &x[i], &y[i]);

        printf("Case #%d: ", t + 1);

        ans1 = ans2 = ans3 = 0;
        forn(i, 2 * s + 1)
            forn(j, 2 * s + 1) {
                z[i][j] = 0;
                c[i][j] = pr(-1, -1);
            }

        forn(i, m) {
            upd(x[i], y[i]);

            sz = 6;
            forn(w, 6) e[w] = get(pr(s + di[w] * (s - 1), s + dj[w] * (s - 1)));
            sort(e, e + sz);
            forab(j, 1, sz)
                if (e[j].i != -1 && e[j] == e[j - 1]) {
                    ans1 = i + 1;
                    break;
                }

            sz = 0;
            obhod();
            sort(e, e + sz);
            forab(j, 2, sz)
                if (e[j].i != -1 && e[j] == e[j - 1] && e[j] == e[j - 2]) {
                    ans2 = i + 1;
                    break;
                }

            if (ans1 != 0 || ans2 != 0) break;
        }

        k = 0;
        forn(i, 2 * s + 1)
            forn(j, 2 * s + 1)
                if ((!z[i][j]) && c[i][j].i == -1) {
                    c[i][j] = pr(i, j);
                    dfs(i, j);
                    k++;
                }

        forba(i, m, 0) {
            k2 = k;
            upd2(x[i], y[i]);
            if (k2 > 1 && k == 1) ans3 = i + 1;
        }

        ans = 0;
        if (ans1 == 0) ans1 = 100500;
        if (ans2 == 0) ans2 = 100500;
        if (ans3 == 0) ans3 = 100500;
        ansmn = min(ans1, min(ans2, ans3));
        if (ansmn != 100500) {
            if (ans1 == ansmn) ans += 1;
            if (ans2 == ansmn) ans += 2;
            if (ans3 == ansmn) ans += 4;
        }

        if (ans == 0) printf("none\n");
        if (ans == 1) printf("bridge in move %d\n", ans1);
        if (ans == 2) printf("fork in move %d\n", ans2);
        if (ans == 3) printf("bridge-fork in move %d\n", ans1);
        if (ans == 4) printf("ring in move %d\n", ans3);
        if (ans == 5) printf("bridge-ring in move %d\n", ans1);
        if (ans == 6) printf("fork-ring in move %d\n", ans2);
        if (ans == 7) printf("bridge-fork-ring in move %d\n", ans1);
    }
    return 0;
}
