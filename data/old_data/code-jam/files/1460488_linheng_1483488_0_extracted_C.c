#include <stdio.h>
#include <memory.h>

int a[10000001];
int f[10000001];
int p[10] = {0,1,10,100,1000,10000,100000,1000000,2000000};

init(){
    int i, n, now, next, last, pre, times;
    memset(a, -1, sizeof(a));
    for (n = 2; n < 7; n++) {
        for (i = p[n]; i < p[n+1]; i++) {
            if (a[i] == -1) {
                pre = i;
                now = i;
                times = n;
                while(times--) {
                    last = now % 10;
                    next = now/10 + last * p[n];
                    if (last == 0) {
                        now = next;
                        continue;
                    }
                    a[pre] = next;
                    pre = next;
                    now = next;
                }
            }
        }
    }
}

int main() {
    int t, t0 = 0;
    init();
    scanf("%d", &t);
    int i;
    while (t--) {
        int p1, p2, res, len, next;
        scanf("%d%d", &p1, &p2);
        memset(f, 0, sizeof(f));
        res = 0;
        if (p1 < 12) p1 = 12;
        for (i = p1; i <= p2; i++)
            if (!f[i]) {
                f[i] = 1;
                next = a[i];
                len = 1;
                while (next != i) {
                    //printf("%d %d\n", i, next);
                    f[next] = 1;
                    if (next <= p2 && next >= p1) len++;
                    next = a[next];
                }
                res += len * (len - 1) / 2;
            }
        printf("Case #%d: %d\n", ++t0, res);
    }
    return 0;
}
