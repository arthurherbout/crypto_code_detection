#include <stdio.h>
#include <string.h>

int T, K, n;
char p[100], d[21][51];
int cnt[21][26];
bool chk[21][21][21][21][21];
int s[6], c[26], ps, t;
int sum[6];

void back(int m) {
	if (m > K) return;
	int i, j;
	for (i = 1; i <= n; i++) {
		s[m] = i;
		if (chk[s[1]][s[2]][s[3]][s[4]][s[5]]) continue;
		chk[s[1]][s[2]][s[3]][s[4]][s[5]] = true;
		for (j = 0; j < 26; j++) c[j] += cnt[i][j];
		ps = 0;
		t = 1;
		for (j = 0; p[j] != '\0'; j++) {
			if (p[j] == '+') {
				ps += t;
				t = 1;
			} else t *= c[p[j] - 'a'];
		}
		ps += t;
		sum[m] += ps;
		sum[m] %= 10009;
		back(m + 1);
		for (j = 0; j < 26; j++) c[j] -= cnt[i][j];
	}
	s[m] = 0;
}

int main() {
	freopen("B-small-attempt0.in", "r", stdin);
	freopen("B-small-attempt0.out", "w", stdout);
	int lT, i, j;
	scanf("%d", &T);
	for (lT = 1; lT <= T; lT++) {
		memset(cnt, 0, sizeof(cnt));
		memset(chk, 0, sizeof(chk));
		memset(sum, 0, sizeof(sum));
		scanf("%s%d%d", p, &K, &n);
		for (i = 1; i <= n; i++) {
			scanf("%s", d[i]);
			for (j = 0; d[i][j] != '\0'; j++) cnt[i][d[i][j] - 'a']++;
		}
		back(1);
		printf("Case #%d:", lT);
		for (i = 1; i <= K; i++)
			printf(" %d", sum[i]);
		printf("\n");
	}
	return 0;
}