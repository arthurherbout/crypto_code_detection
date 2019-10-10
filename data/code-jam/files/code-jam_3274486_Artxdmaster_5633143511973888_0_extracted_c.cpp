#include <bits/stdc++.h>

using namespace std;

#define ll long long

int t, n, k;
double p[55], u;

int main() {
    scanf("%d", &t);
    for (int tc = 0; tc < t; tc++) {
        scanf("%d %d", &n, &k);
        scanf("%lf", &u);
        for (int i = 0; i < n; i++)
            scanf("%lf", &p[i]);

        while (u > 0.00001) {
            sort(p, p+n);
            p[0] += 0.0001;
            u -= 0.0001;
        }

        double ans = 1.0;
        for (int i = 0; i < n; i++)
            ans *= p[i];
        printf("Case #%d: %.6f\n", tc+1, ans);
    }

    return 0;
}


