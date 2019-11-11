#include <stdio.h>
#include <limits.h>

int T, r, c, f;
char m[60][60];
int min;

void back(int p, int q, int dig) {
	if (min <= dig) return;
	if (p == r) {
		min = dig;
		return;
	}
	int i, j;
	for (i = q; m[p][i - 1] == '.' && m[p + 1][i - 1] == '#'; i--);
	if (m[p][i - 1] == '.') {
		for (j = p; j < r && m[j + 1][i - 1] == '.'; j++);
		if (j - p <= f) back(j, i - 1, dig);
	}
	while (true) {
		if (m[p][i - 1] == '.' && m[p + 1][i - 1] == '#') {
			m[p + 1][i - 1] = '.';
			back(p, i, dig + 1);
			m[p + 1][i - 1] = '#';
		}
		if (m[p][i + 1] == '.' && m[p + 1][i + 1] == '#') {
			m[p + 1][i + 1] = '.';
			back(p, i, dig + 1);
			m[p + 1][i + 1] = '#';
		}
		if (m[p][i + 1] == '.' && m[p + 1][i + 1] == '#') i++;
		else break;
	}
	if (m[p][i + 1] == '.') {
		for (j = p; j < r && m[j + 1][i + 1] == '.'; j++);
		if (j - p <= f) back(j, i + 1, dig);
	}
}

int main() {
	int lT, i;
	freopen("B-small-attempt0.in", "r", stdin);
	freopen("B-small-attempt0.out", "w", stdout);
	scanf("%d", &T);
	for (lT = 1; lT <= T; lT++) {
		scanf("%d%d%d", &r, &c, &f);
		for (i = 1; i <= r; i++)
			scanf("%s", m[i] + 1);
		min = INT_MAX;
		back(1, 1, 0);
		if (min == INT_MAX)
			printf("Case #%d: No\n", lT);
		else
			printf("Case #%d: Yes %d\n", lT, min);
	}
	return 0;
}