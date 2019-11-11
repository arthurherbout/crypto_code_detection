#include<stdio.h>
#pragma warning(disable:4996)
int rem[10];

int min(int a, int b) {
	return a < b ? a : b;
}
int main() {
	freopen("A-small-attempt0.in", "r", stdin);
	freopen("output.txt", "w", stdout);
	int m, n, i, j,p;
	int t, ti;
	int temp;
	scanf("%d", &t);
	for (ti = 0; ti < t; ti++) {
		for (i = 0; i < 5; i++) rem[i] = 0;
		printf("Case #%d: ", ti + 1);
		scanf("%d %d", &n, &p);
		for (i = 0; i < n; i++) {
			scanf("%d", &temp);
			rem[temp%p]++;
		}
		int ans = n;
		if (p == 2) {
			ans -= rem[1] / 2;
		}
		else if (p == 3) {
			temp = min(rem[1], rem[2]);
			ans -= temp;
			rem[1] -= temp;
			rem[2] -= temp;
			//0,0,1,2,2,3,4,4,5,6,
			//0,1,2,3,4,5,6,7,8,9,
			//a-(a+2)/3
			if (rem[1] > rem[2]) {
				ans -= rem[1] - (rem[1] + 2) / 3; 
			}
			else if (rem[2] > rem[1]) {
				ans -= rem[2] - (rem[2] + 2) / 3;
			}
		}
		else if (p == 4) {
			temp = min(rem[1], rem[3]);
			ans -= temp;
			rem[1] -= temp;
			rem[3] -= temp;
			ans -= rem[2] / 2;
			rem[2] %= 2;
			if (rem[2] == 0) {
				if (rem[1] == 0) ans -= rem[3] - (rem[3] + 3) / 4;
				else ans -= rem[1] - (rem[1] + 3) / 4;
			}
			else {
				if (rem[1] == 0) {
					if (rem[3] >= 2) {
						ans -= 2;
						rem[3] -= 2;
					}
					else ans -= 1;
					ans -= rem[3] - (rem[3] + 3) / 4;
				}
				else {
					if (rem[1] >= 2) {
						ans -= 2;
						rem[1] -= 2;
					}
					else ans -= 1;
					ans -= rem[3] - (rem[3] + 3) / 4;
				}
			}
		}
		printf("%d\n", ans);
	}
	return 0;
}