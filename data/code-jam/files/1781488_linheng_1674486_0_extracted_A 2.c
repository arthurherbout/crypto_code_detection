#include <stdio.h>
#include <memory.h>

int m[1001][1001];
int size[1001];
int f[1001];
int v[1001];

int dp(int i) {
    int j, p;
    for (j=0; j<size[i]; j++) {
        p = m[i][j];
        f[p] = 1;
        if (v[p] > 0) return 1;
        v[p] = 1;
        if (dp(p))  return 1;
    }
    return 0;
}

int main() {
    int t, t0=0, i, j, n, k, res;
    scanf("%d", &t);
    while (t--) {
        scanf("%d", &n);
        memset(m, 0, sizeof(m));
        for (i=0; i<n; i++) {
            scanf("%d", &k);
            size[i] = k;
            for (j=0; j<k; j++) {
                scanf("%d", &m[i][j]);
            }
        }
        memset(f, 0, sizeof(f));
        int flag = 0;
        for (i=0; i<n; i++) {
            if (f[i] ==0) {
                f[i] = 1;
                memset(v, 0, sizeof(v));
                if (dp(i)) {
                    printf("Case #%d: Yes\n", ++t0);
                    flag = 1;
                    break;
                }
            }
        }
        if (!flag)
            printf("Case #%d: No\n", ++t0);
    }
    return 0;
}
