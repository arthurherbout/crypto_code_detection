#include <cstdio>
#include <cmath>
#include <vector>
using namespace std;
#define MAXN 2005

double d;
int n, a_n;
double t[MAXN], x[MAXN];
double a[255];

int main() {
    freopen("insmall", "r", stdin);
    freopen("outsmall", "w", stdout);
    int cas = 1;
    int T;
    scanf("%d", &T);
    while (T--) {
        scanf("%lf%d%d", &d, &n, &a_n);
        for (int i = 0; i < n; i++)
            scanf("%lf%lf", &t[i], &x[i]);
        for (int i = 0; i < a_n; i++)
            scanf("%lf", &a[i]);
        printf("Case #%d:\n", cas++);
        for (int i = 0; i < a_n; i++) {
            double aa = a[i];
            double ans = sqrt(2 * d / aa);
            if (n == 2) {
                ans = max(ans, (t[1] - t[0]) / (x[1] - x[0])*(d - x[0]) + t[0]);
            }
            printf("%lf\n", ans);
        }
    }
}