#include <bits/stdc++.h>

using namespace std;

const int MAXN = 1000005;

int qu[MAXN], d[MAXN];

int rev(int u) {
    int res = 0;
    while (u > 0) {
        res = res * 10 + u % 10;
        u /= 10;
    }
    return res;
}

int solve(int n) {
    if (n == 1) return 1;
    int fr = 1, re = 1;
    qu[1] = 1;
    d[1] = 1;
    while (fr <= re) {
        int u = qu[fr++];
        int v = u + 1;
        if (d[v] == 0) {
            d[v] = d[u] + 1;
            if (v == n) return d[v];
            qu[++re] = v;
        }
        v = rev(u);
        if (d[v] == 0) {
            d[v] = d[u] + 1;
            if (v == n) return d[v];
            qu[++re] = v;
        }
    }
    return -1;
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    int nTest; scanf("%d", &nTest);
    for (int t = 1; t <= nTest; t++) {
        memset(d, 0, sizeof d);
        int n; scanf("%d", &n);
        printf("Case #%d: %d\n", t, solve(n));
    }

    return 0;
}
