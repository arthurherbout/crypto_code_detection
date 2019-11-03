#include <cstdio>
#include <cstring>
#include <algorithm>

using namespace std;

const int MAX_N = 150;
const int MAX_M = 260;
const int INF = 1000000000;

int cas;
int D, I, m, n;
int a[MAX_N], f[MAX_N][MAX_M];

int main() {
	freopen("D:\\B-small-attempt0.in", "r", stdin);
	freopen("D:\\B-small-attempt0.out", "w", stdout);
	scanf("%d", &cas);
	for (int cc = 1; cc <= cas; ++cc) {
		scanf("%d%d%d%d", &D, &I, &m, &n);
		for (int i = 1; i <= n; ++i) scanf("%d", a + i);
		for (int j = 0; j < MAX_M; ++j) f[1][j] = abs(a[1] - j);
		for (int i = 2; i <= n; ++i) {
			for (int j = 0; j < MAX_M; ++j) {
				f[i][j] = INF;
				f[i][j] = min(f[i][j], f[i - 1][j] + D);
				for (int k = 0; k < MAX_M; ++k) {
					if (abs(j - k) <= m) {
						f[i][j] = min(f[i][j], f[i - 1][k] + abs(a[i] - j));
						continue;
					}
					if (m) {
						f[i][j] = min(f[i][j], f[i - 1][k] + abs(a[i] - j) + ((abs(j - k) / m + (abs(j - k) % m != 0)) - 1) * I);
						continue;
					}
				}
			}
		}
		printf("Case #%d: ", cc);
		int ret = INF;
		for (int j = 0; j < MAX_M; ++j)
			ret = min(ret, f[n][j]);
		printf("%d\n", ret);
	}
	return 0;
}
