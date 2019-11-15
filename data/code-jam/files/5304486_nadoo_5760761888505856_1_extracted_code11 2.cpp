#include<stdio.h>
#pragma warning(disable:4996)


char cake[30][30];

int main() {
	freopen("A-large.in", "r", stdin);
	freopen("output.txt", "w", stdout);
	int m, n, i, j, k;
	int r, c;
	int t, ti;
	scanf("%d", &t);
	for (ti = 0; ti < t; ti++) {
		printf("Case #%d:\n", ti + 1);
		scanf("%d %d", &r, &c);
		for (i = 0; i < r; i++) scanf("%s", cake[i]);
		for (i = 0; i < r - 1; i++) {
			for (j = 0; j < c; j++) {
				if (cake[i][j] != '?' && cake[i + 1][j] == '?')
					cake[i + 1][j] = cake[i][j];
			}
		}
		for (i = r-1; i >0; i--) {
			for (j = 0; j < c; j++) {
				if (cake[i][j] != '?' && cake[i - 1][j] == '?')
					cake[i - 1][j] = cake[i][j];
			}
		}
		for (j = 0; j < c - 1; j++) {
			for (i = 0; i < r; i++) {
				if (cake[i][j] != '?' && cake[i][j + 1] == '?')
					cake[i][j + 1] = cake[i][j];
			}
		}
		for (j = c-1; j >0; j--) {
			for (i = 0; i < r; i++) {
				if (cake[i][j] != '?' && cake[i][j - 1] == '?')
					cake[i][j - 1] = cake[i][j];
			}
		}
		for (i = 0; i < r; i++) printf("%s\n", cake[i]);
	}
	return 0;
}