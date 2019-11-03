#include<stdio.h>
#pragma warning(disable:4996)

int main() {
	freopen("D-small-attempt0.in", "r", stdin);
	freopen("output.txt", "w", stdout);
	int m, n, i, j;
	int t, ti;
	scanf("%d", &t);
	int k, c, s;
	for (ti = 0; ti < t; ti++) {
		printf("Case #%d: ", ti + 1);
		scanf("%d %d %d", &k, &c, &s);
		if (c*s < k) {
			printf("IMPOSSIBLE\n");
			continue;
		}
		for (i = 0; i < k; i += c) {
			long long int sum = 0;
			for (j = 0; j < c; j++) {
				sum *= k;
				if (j + i < k) sum += j + i;
			}
			sum++;
			printf("%lld ", sum);
		}
		printf("\n");
	}
	return 0;
}