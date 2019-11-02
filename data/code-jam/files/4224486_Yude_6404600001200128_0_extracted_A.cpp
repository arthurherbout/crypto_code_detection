#include <bits/stdc++.h>
using namespace std;

int totaltc;
int n;
int m[1000];

int main() {
    int tc = 1;
    scanf("%d", &totaltc);
    while (totaltc--) {
        scanf("%d", &n);
        int maxDrop = 0;
        int totalDrop = 0;
        for (int i = 0; i < n; i++) {
            scanf("%d", &m[i]);
            if (i > 0) {
                maxDrop = max(maxDrop, m[i-1] - m[i]);
                totalDrop += m[i] < m[i-1] ? m[i-1] - m[i] : 0;
            }
        }
        int secondAns = 0;
        for (int i = 0; i < n-1; i++) {
            secondAns += min(m[i], maxDrop);
        }
        printf("Case #%d: %d %d\n", tc++, totalDrop, secondAns);
    }
    return 0;
}
