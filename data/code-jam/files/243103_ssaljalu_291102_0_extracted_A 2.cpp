#include <stdio.h>
#include <algorithm>

#define abs(x) ((x) < 0 ? -(x) : (x))

using namespace std;

class point {
public:
	int r, c;
	point() {}
	point(int tr,int tc) { r = tr; c = tc; }
};

bool operator == (point a, point b) {
	return (a.r == b.r && a.c == b.c);
}

bool cmp (point a, point b) {
	return (a.r > b.r || a.r == b.r && a.c > b.c);
}

int T, r, c;
char t[15][15];
int n;
point q[11000][5];
int hd, tl;
bool s[11000];
int cnt[11000];
bool chk[13][13][13][13][13][13];
int w[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

int main() {
	int lT, i, j, k, l;
	freopen("A-small-attempt0.in", "r", stdin);
	freopen("A-small-attempt0.out", "w", stdout);
	scanf("%d", &T);
	for (lT = 1; lT <= T; lT++) {
		scanf("%d%d", &r, &c);
		memset(q, 0, sizeof(q));
		memset(t, 0, sizeof(t));
		memset(chk, 0, sizeof(chk));
		hd = tl = 0;
		n = 0;
		for (i = 1; i <= r; i++) {
			scanf("%s", t[i] + 1);
			for (j = 1; j <= c; j++)
				if (t[i][j] == 'o' || t[i][j] == 'w')
					q[tl][n++] = point(i, j);
		}
		for (i = 1; i <= r; i++) t[i][0] = t[i][c + 1] = '#';
		for (i = 1; i <= c; i++) t[0][i] = t[r + 1][i] = '#';
		for (i = 0; i < n; i++)
			if (t[q[tl][i].r][q[tl][i].c] == 'o') break;
		if (i == n) {
			printf("Case #%d: 0\n", lT);
			continue;
		}
		sort(q[tl], q[tl] + n, cmp);
		s[tl] = true;
		tl++;
		while (hd < tl) {
			for (i = 0; i < n; i++) {
				for (j = 0; j < 4; j++) {
					for (k = 0; k < n; k++) {
						if (i == k) {
							q[tl][k] = point(q[hd][k].r + w[j][0], q[hd][k].c + w[j][1]);
							if (t[q[tl][k].r][q[tl][k].c] == '#' || t[q[hd][k].r + w[j ^ 1][0]][q[hd][k].c + w[j ^ 1][1]] == '#') break;
							for (l = 0; l < n; l++)
								if (q[hd][l] == q[tl][k] || q[hd][l] == point(q[hd][k].r + w[j ^ 1][0], q[hd][k].c + w[j ^ 1][1])) break;
							if (l < n) break;
						} else q[tl][k] = q[hd][k];
					}
					if (k < n) continue;
					s[tl] = true;
					if (n != 1) {
						for (k = 0; k < n; k++) {
							for (l = 0; l < n; l++)
								if (abs(q[tl][k].r - q[tl][l].r) + abs(q[tl][k].c - q[tl][l].c) == 1) break;
							if (l == n) break;
						}
						if (k < n) s[tl] = false;
					}
					if (!s[hd] && !s[tl]) continue;
					sort(q[tl], q[tl] + n, cmp);
					/*
					if (chk[q[tl][0].r][q[tl][0].c][q[tl][1].r][q[tl][1].c]) continue;
					chk[q[tl][0].r][q[tl][0].c][q[tl][1].r][q[tl][1].c] = true;
					*/
					if (chk[q[tl][0].r][q[tl][0].c][q[tl][1].r][q[tl][1].c][q[tl][2].r][q[tl][2].c]) continue;
					chk[q[tl][0].r][q[tl][0].c][q[tl][1].r][q[tl][1].c][q[tl][2].r][q[tl][2].c] = true;
					cnt[tl] = cnt[hd] + 1;
					for (k = 0; k < n; k++)
						if (t[q[tl][k].r][q[tl][k].c] != 'x' && t[q[tl][k].r][q[tl][k].c] != 'w') break;
					if (k == n) break;
					tl++;
				}
				if (j < 4) break;
			}
			if (i < n) break;
			hd++;
		}
		if (hd < tl) printf("Case #%d: %d\n", lT, cnt[tl]);
		else printf("Case #%d: -1\n", lT);
	}
	return 0;
}