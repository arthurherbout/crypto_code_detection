#include <cstdio>
#include <cmath>
#include <algorithm>

using namespace std;

const int MAXN = 500 + 10;
int a[MAXN][MAXN];

bool eq(double x, double y) {
	return abs(x - y) < 1e-4;
}

bool gao(int x, int y, int len) {
	long long tot = 0;
	long long cx = 0, cy = 0;
	for (int i = x; i < x + len; ++i) {
		for (int j = y; j < y + len; ++j) {
			if ((i == x || i == x + len - 1) && (j == y || j == y + len - 1)) {
				continue;
			}
			tot += a[i][j];
			cx += i * a[i][j];
			cy += j * a[i][j];
		}
	}
	return cx * 2 == tot * ((long long)x * 2 + len - 1) && cy * 2 == tot * ((long long)y * 2 + len - 1);
//	return eq(cx, x + len * 0.5 - 0.5) && eq(cy, y + len * 0.5 - .5);
}

int main() {
	int nCase;

	scanf("%d", &nCase);

	for (int re = 1; re <= nCase; ++re) {
		int r, c, d, ans = -1;
		scanf("%d%d%d", &r, &c, &d);
		for (int i = 0; i < r; ++i) {
			for (int j = 0; j < c; ++j) {
				scanf("%1d", &a[i][j]);
			}
		}

		for (int i = 0; i < r ; ++i) {
			for (int j = 0; j < c; ++j) {
				for (int k = 3; i + k <= r && j + k <= c; ++k) {
					if (gao(i, j, k)) {
						ans = max(ans, k);
					}
				}
			}
		}

		if (ans == -1) {
			printf("Case #%d: IMPOSSIBLE\n", re);
			continue;
		}
			

		printf("Case #%d: %d\n", re, ans);
	}

	return 0;
}
