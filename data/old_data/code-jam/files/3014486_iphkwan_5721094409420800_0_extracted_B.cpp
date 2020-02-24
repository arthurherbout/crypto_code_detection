#include <iostream>
#include <fstream>
#include <cstdio>
#include <string.h>
using namespace std;

int a[1010], b[1010], c[1010], cnt;

void left_merge(int l, int r) {
    int mid, i, j, tmp;
    if (r > l) {
        mid = (l + r) / 2;
        left_merge(l, mid);
        left_merge(mid + 1, r);
        tmp = l;
        for (i = l, j = mid + 1; i <= mid && j <= r; ) {
            if (b[i] > b[j]) {
                c[tmp++] = b[j++];
                cnt += (mid - i + 1);
            }
            else
                c[tmp++] = b[i++];
        }
        if (j <= r) {
            for (; j < r; ++j)
                c[tmp++] = b[j];
        }
        else {
            for (; i <= mid; ++i)
                c[tmp++] = b[i];
        }
        for (i = l; i <= r; ++i)
            b[i] = c[i];
    }
}

int main() {
    freopen("b.in","r", stdin);
    freopen("ans1.txt", "w", stdout);

    int t, n;
    scanf("%d", &t);
    for (int z = 1; z <= t; z++) {
        scanf("%d", &n);

        for (int i = 0; i < n; i++) {
            scanf("%d", &a[i]);
        }
        int ans = -1;
        for (int i = 1; i <= n; i++) {
            for (int j = 0; j < i; j++)
                b[j] = a[j];
            for (int j = i; j < n; j++)
                b[j] = a[n - j + i - 1];

            cnt = 0;
            left_merge(0, i - 1);
            left_merge(i, n - 1);
            if (ans == -1)
                ans = cnt;
            else
                ans = ans < cnt ? ans : cnt;
        }
        printf("Case #%d: %d\n", z, ans);
    }
    return 0;
}
