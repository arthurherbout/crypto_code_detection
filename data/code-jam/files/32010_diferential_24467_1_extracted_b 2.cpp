#include <cstdio>
#include <cstdlib>

#include <iostream>
#include <set>
#include <map>
#include <string>
#include <algorithm>
#include <sstream>
#include <vector>

using namespace std;

map<string,int>colors;

struct offer {
    int l, r, color;

    bool operator < (const offer& oth) const {
        return l < oth.l;
    }
};

vector<offer> of;
int N;

void read_one()
{
    colors.clear();
    cin >> N;
    of.resize(N);
    int ncol = 1, i;
    for (i = 0; i < N; i++) {
        string color;
        cin >> color;
        cin >> of[i].l >> of[i].r;

        if (colors[color] == 0) {
            colors[color] = ncol ++;
        }
        of[i].color = colors[color];
    }
}

int done[301][301][301];

void solve_one()
{
    sort(of.begin(), of.end());
    int i, j, k, l;
    int best = 10000;
    
    memset (done, 0, sizeof(done));

    for (i = 0; i < N; i++)
    for (j = i; j < N; j++)
    for (k = j; k < N; k++) {
        int ci = of[i].color, cj = of[j].color, ck = of[k].color;
        if (done[ci][cj][ck])
            continue;

        int cost = 0, pos = 0, npos;
        for (l = 0; pos < 10000 && l < N; pos = npos, cost++)
        {
            npos = pos;
            for (; l < N && of[l].l <= pos + 1; l++)
            if (of[l].color == ci || of[l].color == cj || of[l].color == ck)
                npos = max(npos, of[l].r);
            if (npos == pos)
                break;
        }
        if (pos >= 10000 && best > cost) {
            best = cost;
        }

        done[ci][cj][ck] = done[ci][ck][cj] = done[cj][ci][ck] = done[cj][ck][ci] = done[ck][ci][cj] = done[ck][cj][ci] = 1;
    }
    if (best >= 1000)
        printf ("IMPOSSIBLE");
    else
        printf ("%d", best);
}

int main(void)
{
    int T, i;

    for(scanf("%d\n", &T), i = 1; i <= T; i++) {
        read_one();
        printf ("Case #%d: ", i);
        solve_one();
        printf ("\n");
    }
}

