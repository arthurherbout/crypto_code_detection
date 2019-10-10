#include <cstdio>
#include <cmath>
#include <algorithm>
using namespace std;
#define MAXN 110
#define INF 1e12
int cases, n;
double x[MAXN], y[MAXN], r[MAXN], ans, t;
int main() {
	freopen("D-small.in", "r", stdin);
	freopen("D-small.out", "w", stdout);
	scanf("%d", &cases);
	for (int cc = 1; cc <= cases; cc++) {
		scanf("%d", &n);
		for (int i = 0; i < n; i++) scanf("%lf%lf%lf", &x[i], &y[i], &r[i]);
		printf("Case #%d: ", cc);
		if (n == 1) printf("%.6lf\n", r[0]);
		if (n == 2) printf("%.6lf\n", r[0] > r[1] ? r[0] : r[1]);
		if (n == 3) {
			ans = INF;
			for (int i = 0; i < 3; i++) {
				t = (sqrt((x[i] - x[(i + 1) % 3]) * (x[i] - x[(i + 1) % 3]) + (y[i] - y[(i + 1) % 3]) * (y[i] - y[(i + 1) % 3])) + r[i] + r[(i + 1) % 3]) / 2;
				t = max(t, r[(i + 2) % 3]);
				ans = min(ans, t);
			}
			printf("%.6lf\n", ans);
		}
	}
	fclose(stdin); fclose(stdout);
	return 0;
}