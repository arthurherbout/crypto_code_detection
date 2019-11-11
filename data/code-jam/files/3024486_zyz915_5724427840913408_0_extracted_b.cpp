#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <map>
#include <vector>
#include <string>
#include <set>
#include <algorithm>

using namespace std;

const int N = 110;
int h[N], g[N], f[N][10 * N][2];

void run(int cas) {
	int p, q, n;
	scanf("%d%d%d", &p, &q, &n);
	for (int i = 0; i < n; i++)
		scanf("%d%d", &h[i], &g[i]);
	memset(f, -1, sizeof(f));
	f[0][0][0] = 0;
	for (int i = 0; i < n; i++) // already kill
		for (int j = 0; j <= 10 * (i + 1); j++) // additional attacks
			for (int t = 0; t < 2; t++) // turn
				if (f[i][j][t] > -1) {
					//printf("%d %d %d -> %d\n", i, j, t, f[i][j][t]);
					for (int cons = 0; cons <= j; cons++) {
						int remhp = h[i] - cons * p;
						if (remhp <= 0) {
							f[i + 1][j - cons][t] = max(f[i + 1][j - cons][t], f[i][j][t] + g[i]);
							break;
						}
						for (int miss = 0; miss <= 20; miss++) {
							// simulation
							int tt = t, hp = remhp, mm = miss;
							while (hp > 0) {
								if (tt == 0) {
									if (mm == 0)
										hp -= p;
									if (mm > 0) mm--;
								} else {
									hp -= q;
								}
								tt = 1 - tt;
							}
							int j1 = j - cons + (miss - mm);
							if (tt == 1) // get gold
								f[i + 1][j1][tt] = max(f[i + 1][j1][tt], f[i][j][t] + g[i]);
							else
								f[i + 1][j1][tt] = max(f[i + 1][j1][tt], f[i][j][t]);
							if (mm > 0) break;
						}
					}
				}
	int ans = 0;
	for (int j = 0; j <= 10 * n; j++)
		for (int t = 0; t < 2; t++)
			ans = max(ans, f[n][j][t]);
	printf("Case #%d: %d\n", cas, ans);
}

int main() {
    int tt, cas;
    scanf("%d", &tt);
    for (cas = 1; cas <= tt; cas++)
        run(cas);
    return 0;
}
