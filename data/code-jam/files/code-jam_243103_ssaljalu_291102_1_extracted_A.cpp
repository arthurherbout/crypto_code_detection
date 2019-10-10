#include <stdio.h>
#include <queue>
#include <map>
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

class status {
public:
	point p[5];
};

bool operator < (status a, status b) {
	for (int i = 0; i < 5; i++) {
		if (cmp(a.p[i], b.p[i])) return true;
		if (cmp(b.p[i], a.p[i])) return false;
	}
	return false;
}

int T, r, c;
char t[15][15];
int n;
status hd, tl;
bool s_hd, s_tl;
int cnt_hd, cnt_tl;
queue <status> q;
queue <bool> s;
queue <int> cnt;
map <status, bool> chk;
int w[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

int main() {
	int lT, i, j, k, l;
	freopen("A-large.in", "r", stdin);
	freopen("A-large1.out", "w", stdout);
	scanf("%d", &T);
	for (lT = 1; lT <= T; lT++) {
		scanf("%d%d", &r, &c);
		memset(t, 0, sizeof(t));
		while (!q.empty()) { q.pop(); s.pop(); cnt.pop(); }
		chk.clear();
		n = 0;
		for (i = 1; i <= r; i++) {
			scanf("%s", t[i] + 1);
			for (j = 1; j <= c; j++)
				if (t[i][j] == 'o' || t[i][j] == 'w')
					tl.p[n++] = point(i, j);
		}
		for (i = 1; i <= r; i++) t[i][0] = t[i][c + 1] = '#';
		for (i = 1; i <= c; i++) t[0][i] = t[r + 1][i] = '#';
		for (i = 0; i < n; i++)
			if (t[tl.p[i].r][tl.p[i].c] == 'o') break;
		if (i == n) {
			printf("Case #%d: 0\n", lT);
			continue;
		}
		sort(tl.p, tl.p + n, cmp);
		s_tl = true;
		cnt_tl = 0;
		chk[tl] = true;
		q.push(tl);
		s.push(s_tl);
		cnt.push(cnt_tl);
		while (!q.empty()) {
			hd = q.front();
			s_hd = s.front();
			cnt_hd = cnt.front();
			for (i = 0; i < n; i++) {
				for (j = 0; j < 4; j++) {
					for (k = 0; k < n; k++) {
						if (i == k) {
							tl.p[k] = point(hd.p[k].r + w[j][0], hd.p[k].c + w[j][1]);
							if (t[tl.p[k].r][tl.p[k].c] == '#' || t[hd.p[k].r + w[j ^ 1][0]][hd.p[k].c + w[j ^ 1][1]] == '#') break;
							for (l = 0; l < n; l++)
								if (hd.p[l] == tl.p[k] || hd.p[l] == point(hd.p[k].r + w[j ^ 1][0], hd.p[k].c + w[j ^ 1][1])) break;
							if (l < n) break;
						} else tl.p[k] = hd.p[k];
					}
					if (k < n) continue;
					s_tl = true;
					if (n != 1) {
						for (k = 0; k < n; k++) {
							for (l = 0; l < n; l++)
								if (abs(tl.p[k].r - tl.p[l].r) + abs(tl.p[k].c - tl.p[l].c) == 1) break;
							if (l == n) break;
						}
						if (k < n) s_tl = false;
					}
					if (!s_hd && !s_tl) continue;
					sort(tl.p, tl.p + n, cmp);
					if (chk[tl]) continue;
					chk[tl] = true;
					cnt_tl = cnt_hd + 1;
					for (k = 0; k < n; k++)
						if (t[tl.p[k].r][tl.p[k].c] != 'x' && t[tl.p[k].r][tl.p[k].c] != 'w') break;
					if (k == n) break;
					q.push(tl);
					s.push(s_tl);
					cnt.push(cnt_tl);
				}
				if (j < 4) break;
			}
			if (i < n) break;
			q.pop(); s.pop(); cnt.pop();
		}
		if (!q.empty()) printf("Case #%d: %d\n", lT, cnt_tl);
		else printf("Case #%d: -1\n", lT);
	}
	return 0;
}