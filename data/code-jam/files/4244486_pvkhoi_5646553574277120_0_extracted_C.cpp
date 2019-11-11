#include <bits/stdc++.h>

using namespace std;

const int MAXN = 10, MAXV = 1000;

bool check[MAXV], can[MAXV];
int a[MAXN];
int c, d, v, res, n;

void dq(int pos, int sum) {
    if (pos == d) {
        can[sum] = true;
        return;
    }
    for (int i = 0; i < 2; i++) {
        dq(pos+1, sum + a[pos]*i);
    }
}

bool unable() {
    for (int i = 1; i <= v; i++)
        if (can[i] == false) return true;
    return false;
}

int solve() {
    memset(check, false, sizeof check);
    memset(can, 0, sizeof can);
    for (int i = 0; i < d; i++) check[a[i]] = true;
    dq(0, 0);
    int res = 0;
    while (unable()) {
        res++;
        for (int j = 1; j <= v; j++) {
            if (can[j]) continue;
            a[d++] = j;
            break;
        }
        dq(0, 0);
    }
    return res;
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    int nTest; scanf("%d", &nTest);
    for (int t = 1; t <= nTest; t++) {
        scanf("%d%d%d", &c, &d, &v);
        for (int i = 0; i < d; i++) scanf("%d", &a[i]);
        printf("Case #%d: %d\n", t, solve());
    }

    return 0;
}
