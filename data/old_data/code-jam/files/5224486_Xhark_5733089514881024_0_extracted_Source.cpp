#include <stdio.h>
#include <algorithm>
#include <vector>
#include <memory.h>
#include <math.h>
#include <string.h>
#include <map>
#include <string>
#include <set>

using namespace std;

char dat[10005];
int N;
int dyn[10005][10005];
int tmp[10005][10005];
int main() {
	freopen("A-small-attempt0.in", "r", stdin);
	freopen("A-small-attempt0.out", "w", stdout);
	int T;
	scanf("%d", &T);
	int N;
	for (int cs = 1; cs <= T; cs++) {
		memset(dyn, 0, sizeof(dyn));
		memset(tmp, 0, sizeof(tmp));
		scanf("%s", dat);
		N = strlen(dat);
		for (int i = 0; i < N; i++) {
			for (int j = i; j < N; j++) {
				if (i == j) {
					dyn[i][j] = dat[i] - '0';
					tmp[i][j] = i;
				}
				else {
					for (int k = tmp[i][j - 1]; k <= j; k++) {
						int pp = dyn[i][k - 1] + dyn[k + 1][j] + dat[k] - '0';
						if (k == tmp[i][j - 1]) {
							dyn[i][j] = pp;
							tmp[i][j] = k;
						}
						else if (dyn[i][j] > pp) {
							dyn[i][j] = pp;
							tmp[i][j] = k;
						}
						else {
							break;
						}
					}
				}
			}
		}
		printf("Case #%d: %d\n", cs, dyn[0][N-1]);
	}
	return 0;
}