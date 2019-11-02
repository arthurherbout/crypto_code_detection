#include <bits/stdc++.h>

using namespace std;

int r, c, w;

int solve() {
    int res = c / w * r + (w - 1);
    if (c % w > 0) res++;
    return res;
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    int nTest; scanf("%d", &nTest);
    for (int t = 1; t <= nTest; t++) {
        scanf("%d%d%d", &r, &c, &w);
        printf("Case #%d: %d\n", t, solve());
    }

    return 0;
}
