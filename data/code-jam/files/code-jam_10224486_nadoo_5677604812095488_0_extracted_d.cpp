#include<stdio.h>
#include<algorithm>

using namespace std;

double dp[210];
double arr[210];
double calc(int k) {
	int m, n, i, j;
	for (i = 0; i <= k; i++) dp[i] = 0;
	dp[0] = 1 - arr[0];
	dp[1] = arr[0];
	for (i = 1; i < k; i++) {
		for (j = k-1; j >= 0; j--) {
			dp[j + 1] += dp[j] * arr[i];
			dp[j] = dp[j] * (1 - arr[i]);
		}
	}
	return dp[k / 2];
}

int main() {
	freopen("B-small-attempt0.in", "r", stdin);
	freopen("B-small.out", "w", stdout);
	int m, n, i, j,k;
	int t, ti;
	scanf("%d", &t);
	for (ti = 0; ti < t; ti++) {
		printf("Case #%d: ", ti + 1);
		scanf("%d %d", &n, &k);
		for (i = 0; i < n; i++) scanf("%lf", &arr[i]);
		sort(arr, arr + n);
		for (i = k / 2; i < k; i++) {
			double temp = arr[i];
			arr[i] = arr[n - (i - k / 2) - 1];
			arr[n - (i - k / 2) - 1] = temp;
		}
		printf("%f\n", calc(k));
	}
	
	return 0;
}