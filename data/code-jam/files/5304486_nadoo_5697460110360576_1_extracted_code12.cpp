#include<stdio.h>
#include<algorithm>
#pragma warning(disable:4996)

int need[60];
int ingr[60][60];
int count[60][60][2];
int ptr[60];

int get_min(int n) {
	int i;
	int ans = 0;
	for (i = 0; i < n; i++) {
		if (count[ans][ptr[ans]][1] > count[i][ptr[i]][1]) ans = i;
	}
	return ans;
}

int main() {
	freopen("B-large.in", "r", stdin);
	freopen("output.txt", "w", stdout);
	int m, n, i, j, k,p;
	int t, ti;
	scanf("%d", &t);
	for (ti = 0; ti < t; ti++) {
		printf("Case #%d: ", ti + 1);
		scanf("%d %d", &n, &p);
		for (i = 0; i < n; i++) ptr[i] = 0;
		for (i = 0; i < n; i++) scanf("%d", &need[i]);
		for (i = 0; i < n; i++) {
			for (j = 0; j < p; j++) scanf("%d", &ingr[i][j]);
		}
		for (i = 0; i < n; i++) {
			std::sort(ingr[i], ingr[i] + p);
		}
		for (i = 0; i < n; i++) {
			for (j = 0; j < p; j++) { // count * 0.9 <= ingr[i][j] <= count * 1.1
				//ingr[i][j]*10/11 /need[i] <= count <= ingr[i][j]*10/9 / need[i]
				count[i][j][0] = (ingr[i][j] * 10 +11*need[i]-1) / 11 / need[i];
				count[i][j][1] = ingr[i][j] * 10 / 9 / need[i];
				if (count[i][j][0] > count[i][j][1]) {
					count[i][j][0] = -1;
				}
			}
		}
		int ans = 0;
		while (1) {
			for (i = 0; i < n; i++) {
				while (!(ptr[i] >= p) && count[i][ptr[i]][0] == -1) ptr[i]++;
				if (ptr[i] >= p)
					break;
			}
			if (i < n) break;
			int min = get_min(n);
			int fail = 0;
			for (i = 0; i < n; i++) {
				if (count[i][ptr[i]][0] > count[min][ptr[min]][1]) {
					fail = 1;
					break;
				}
			}
			if (fail == 1) {
				ptr[min]++;
			}
			else {
				ans++;
				for (i = 0; i < n; i++) ptr[i]++;
			}
		}
		printf("%d\n", ans);

	}
	return 0;
}