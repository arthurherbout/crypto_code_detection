#include <iostream>
#include <cstdio>
#include <string.h>
#include <queue>
#include <map>
#include <string>
#include <algorithm>
using namespace std;
#define MAXN 2000
#define OFFSET 1000

int ww[MAXN], tmp[MAXN];

struct Node
{
    int d, w, e, s;
}node[210];
int tot;
bool cmp(Node a, Node b)
{
    return a.d < b.d;
}
bool judge(int tw, int te, int s)
{
    for (int i = tw; i <= te; i++)
        tmp[i] = max(tmp[i], s);
    for (int i = tw; i <= te; i++)
        if(s > ww[i])
            return true;
    return false;
}
void refresh()
{
    for (int i = 0; i < MAXN; i++)
        ww[i] = max(ww[i], tmp[i]);
}
int main()
{
    freopen("t.in", "r", stdin);
    freopen("ans.txt", "w", stdout);
    int t, n;
    int td, tn, tw, te, ts, tdd, tdp, tds;
    scanf("%d", &t);
    for (int z = 1; z <= t; z++)
    {
        memset(ww, 0, sizeof(ww));
        tot = 0;
        scanf("%d", &n);
        for (int i = 0; i < n; i++)
        {
            scanf("%d%d%d%d%d%d%d%d", &td, &tn, &tw, &te, &ts, &tdd, &tdp, &tds);
            tw *= 2;
            te *= 2;
            tdp *= 2;
            tw += OFFSET;
            te += OFFSET;
            for (int j = 0; j < tn; j++)
            {
                node[tot].d = td;
                node[tot].w = tw;
                node[tot].e = te;
                node[tot++].s = ts;
                td += tdd;
                tw += tdp;
                te += tdp;
                ts += tds;
            }
        }
        sort(node, node + tot, cmp);
        //printf("tot = %d\n", tot);
        int ans = 0;
        int cntd;
        for (int i = 0; i < tot; i++)
        {
            cntd = node[i].d;
            memset(tmp, 0, sizeof(tmp));
            while(i < tot && node[i].d == cntd)
            {
                //printf("i = %d, w = %d, e = %d, s = %d\n", i, node[i].w - OFFSET, node[i].e - OFFSET, node[i].s);
                if(judge(node[i].w, node[i].e, node[i].s))
                {
                    ans++;
                    //printf("w = %d, e = %d, s = %d\n", node[i].w - OFFSET, node[i].e - OFFSET, node[i].s);
                }
                i++;
            }
            i--;
            refresh();
        }
        printf("Case #%d: %d\n", z, ans);
    }
    return 0;
}
