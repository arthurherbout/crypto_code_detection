#include<stdio.h>
#pragma warning(disable:4996) 

int arr[1001][2];
int cus[1001];
int pos[1001];

int main() {
	freopen("B-small-attempt0.in", "r", stdin);
	freopen("output.txt", "w", stdout);
	int m, n, i, j,c;
	int t, ti;
	scanf("%d", &t);
	for (ti = 0; ti < t; ti++) {
		for (i = 1; i <= 1000; i++) {
			cus[i] = 0;
			pos[i] = 0;
		}
		printf("Case #%d: ", ti + 1);
		scanf("%d %d %d", &n, &c, &m);
		for (i = 0; i < m; i++) {
			scanf("%d %d", &arr[i][0], &arr[i][1]);
			pos[arr[i][0]]++;
			cus[arr[i][1]]++;
		}
		int max = 0;
		for (i = 1; i <= c; i++) {
			if (max < cus[i]) max = cus[i];
		}
		int sum = 0;
		for (i = 1; i <= n; i++) {
			sum += pos[i];
			if (max < (pos[i] + i - 1) / i) max = (pos[i] + i - 1) / i;
		}
		int promote = 0;
		for (i = 1; i <= n; i++) {
			if (pos[i] > max) promote += pos[i] - max;
		}
		printf("%d %d\n", max, promote);
	}
	return 0;
}