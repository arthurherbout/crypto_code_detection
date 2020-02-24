#include <cstdio>
#include <cstring>

char st[110][20];
char buf[50], match[20];
int mark[110];

int main()
{
	freopen("B.in", "r", stdin);
	freopen("B.out", "w", stdout);
	int cases, n, m;
	scanf("%d", &cases);
	for (int cc = 1; cc <= cases; ++cc) {
		scanf("%d%d", &n, &m);
		gets(buf);
		for (int i = 0; i < n; ++i) gets(st[i]);
		printf("Case #%d:", cc);
		while (m--) {
			gets(buf);
			int ans = -1;
			char answord[50];
			for (int i = 0; i < n; ++i) {
				memset(mark, 0, sizeof(mark));
				int cnt = 0;
				for (int j = 0; j < n; ++j) {
					if (i == j) continue;
					if (strlen(st[i]) != strlen(st[j])) {
						mark[j] = 1;
						++cnt;
					}
				}
				memset(match, '.', sizeof(match));
				int cnt1 = 0;
				for (int j = 0; j < 26; ++j) {
					if (strchr(st[i], buf[j])) {
						for (int k = 0; st[i][k]; ++k) {
							if (buf[j] == st[i][k]) match[k] = buf[j];
						}
						for (int k = 0; k < n; ++k) {
							if (k == i) continue;
							if (!mark[k]) {
								for (int l = 0; st[i][l]; ++l) {
									if (match[l] == '.' || match[l] == st[k][l]) continue;
									mark[k] = 1;
									++cnt;
									break;
								}
							}
						}
					} else {
						int flag = 0;
						for (int k = 0; k < n; ++k) {
							if (i != k && !mark[k] && strchr(st[k], buf[j])) {
								++cnt;
								mark[k] = 1;
								flag = 1;
							}
						}
						cnt1 += flag;
					}
					if (cnt == n - 1) {
						if (ans < cnt1) {
							ans = cnt1;
							memcpy(answord, st[i], sizeof(st[i]));
						}
						break;
					}
				}
			}
			printf(" %s", answord);
		}
		puts("");
	}
	return 0;
}
