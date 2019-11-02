#include<stdio.h>
#pragma warning(disable:4996)

long long int conv(long long int num,int base) {
	int m, n, i, j;
	long long int cur=1;
	long long int ans = 0;
	while (num) {
		if (num % 10) ans += cur;
		cur *= base;
		num /= 10;
	}
	return ans;
}

int main() {
	freopen("output1.txt", "w", stdout);
	int m, n, i, j;
	int t, ti;
	scanf("%d", &t);
	for (ti = 0; ti < t; ti++) {
		printf("Case #%d: \n", ti + 1);
		scanf("%d %d", &n, &j);
		for (i = 0; i < j; i++) {
			long long int num = 1;
			long long int cur = 10;
			int dex = i;
			int len = 1;
			while (dex) {
				if (dex % 2) num += cur;
				dex /= 2;
				cur *= 10;
				len++;
			}
			num += cur;
			len++;
			printf("%lld", num);
			for (m = 0; m < n - 2 * len; m++) printf("0");
			printf("%lld ", num);
			for (m = 2; m <= 10; m++) printf("%lld ", conv(num,m));
			printf("\n");
		}
	}
	return 0;

}