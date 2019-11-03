#include <stdio.h>

#define SQR(x) ((x) * (x))

inline int sign(int x) {
	if (x == 0) return 0;
	if (x > 0) return 1;
	if (x < 0) return -1;
}

inline int abs(int x) {
	if (x > 0) return x; else return -x;
}

const int MAXN = 60;

int n, m, d;
char map[MAXN][MAXN];
int posx, posy;
int pts[20000][2];

int main() {
	int cases;
	scanf("%d", &cases);
	for (int k = 0; k < cases; ++k) {
		scanf("%d%d%d", &n, &m, &d);
		for (int i = 0; i < n; ++i) scanf("%s", map[i]);
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < m; ++j) {
				if (map[i][j] == 'X') {
					posx = i, posy = j;
				}
			}
		}
		int px = 2 * (posx - 1) + 1, py = 2 * (posy - 1) + 1;
		n = 2 * (n - 2), m = 2 * (m - 2);
		fprintf(stderr, "n = %d, m = %d, px = %d, py = %d\n", n, m, px, py);
		int top = 0;
		for (int i = -d * 2; i <= d * 2; ++i) {
			for (int j = -d * 2; j <= d * 2; ++j) {
				int newx, newy;
				newx = n * i, newy = m * j;
				if (SQR(newx) + SQR(newy) <= SQR(d)) {
					pts[top][0] = newx, pts[top][1] = newy;
					++top;
				}
				newx = n * i - px, newy = m * j;
				if (SQR(newx) + SQR(newy) <= SQR(d)) {
					pts[top][0] = newx, pts[top][1] = newy;
					++top;
				}
				newx = n * i, newy = m * j -  py;
				if (SQR(newx) + SQR(newy) <= SQR(d)) {
					pts[top][0] = newx, pts[top][1] = newy;
					++top;
				}
				newx = n * i - px, newy = m * j - py;
				if (SQR(newx) + SQR(newy) <= SQR(d)) {
					pts[top][0] = newx, pts[top][1] = newy;
					++top;
				}
			}
		}
		fprintf(stderr, "top = %d\n", top);
		for (int i = 0; i < top; ++i) {
//			fprintf(stderr, "(%d, %d)\n", pts[i][0], pts[i][1]);
		}
		int ans = 0;
		for (int i = 0; i < top; ++i) {
			bool flag = true;
			for (int j = 0; j < top; ++j) {
				if (i == j) continue;
				if (sign(pts[i][0]) != sign(pts[j][0])) continue;
				if (sign(pts[i][1]) != sign(pts[j][1])) continue;
				if (abs(pts[i][0]) < abs(pts[j][0])) continue;
				if (abs(pts[i][1]) < abs(pts[j][1])) continue;
				if (pts[i][0] * pts[j][1] == pts[i][1] * pts[j][0]) {
					flag = false;
					break;
				}
			}
			if (flag)
				++ans;
//			else {printf("---(%d, %d)\n", pts[i][0], pts[i][1]); }
		}
		printf("Case #%d: %d\n", k + 1, ans - 1);
	}
	return 0;
}

