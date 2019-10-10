#include <iostream>
#include <cstdio>
#include <string.h>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

int g[1050][1050], deg[1050], t, n, tx, ty, sbt[1050];

int subtree(int cur, int fa) {
    int ret = 1;
    for (int i = 0; i < deg[cur]; i++) {
        if (g[cur][i] != fa)
            ret += subtree(g[cur][i], cur);
    }
    sbt[cur] = ret;
    return ret;
}

int dfs(int cur, int fa) {
    vector<int>  tmpa;
    int tot = 0;
    for (int i = 0; i < deg[cur]; i++) {
        if (g[cur][i] != fa) {
            tmpa.push_back(dfs(g[cur][i], cur));
            tot++;
        }
    }
    if (tmpa.size() < 2)
        return 1;
    sort(tmpa.begin(), tmpa.end());
    return tmpa[tot - 1] + tmpa[tot - 2] + 1;
}

int main() {
    freopen("tb.in", "r", stdin);
    freopen("ansb.txt", "w", stdout);
    scanf("%d", &t);
    for (int z = 1; z <= t; z++) {
        scanf("%d", &n);
        memset(deg, 0, sizeof(deg));
        for (int i = 1; i < n; i++) {
            scanf("%d%d", &tx, &ty);
            g[tx][ deg[tx]++ ] = ty;
            g[ty][ deg[ty]++ ] = tx;
        }
        int ans = 1001;
        for (int i = 1; i <= n; i++) {
            subtree(i, 0);
            ans = min(ans, n - dfs(i, 0));
        }
        printf("Case #%d: %d\n", z, ans);
    }
    return 0;
}
