#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>

#define MAXN 20

typedef long long ll;

struct Point {
	ll x, y;
}point[MAXN], npt[MAXN], plo[MAXN];

ll dlcmp(ll x) {
	return x < 0 ? -1: x > 0;
}

ll cross(Point a, Point b, Point s) {
	ll x1 = a.x - s.x, y1 = a.y - s.y;
	ll x2 = b.x - s.x, y2 = b.y - s.y;
	return x1 * y2 - x2 * y1;
}

ll dis(Point a, Point b) {
	return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

ll gcross(ll x1, ll y1, ll x2, ll y2) {
	return x1 * y2 - x2 * y1;
}

int cmp(Point a, Point b) {
	if(dlcmp(cross(a, b, npt[0])) == 1|| (dlcmp(cross(a, b, npt[0])) == 0 
		&& dis(npt[0], a) < dis(npt[0], b))) {
 		return 1;
 	}
	else return 0;
}

int graham_scan(int num) {
	if (num <= 1) return num;
	int top, t = 0;
	for (int i = 1; i < num; ++i) {
		if (dlcmp(npt[i].y - npt[t].y) == 1 || (dlcmp(npt[i].y - npt[t].y) == 0 
			&& dlcmp(npt[i].x - npt[t].x) == -1))
			t = i;
	}
	std::swap(npt[0], npt[t]);
	std::sort(npt + 1, npt + num, cmp);
	top = 2;
	for (int i = 0; i < 2; ++ i) {
		plo[i] = npt[i];
	}
	for (int i = 2; i < num; ++ i) {
		while(top > 1 && dlcmp(cross(plo[top - 1], npt[i], plo[top - 2])) <= 0) {
			--top;
		}
		plo[top++] = npt[i];
	}
	return top;
}

int on_segment(Point a, Point b, Point k) {
	if(gcross(k.x - a.x, k.y - a.y, b.x - a.x, b.y - a.y) == 0 &&
		k.x >= std::min(a.x, b.x) && k.x <= std::max(a.x, b.x) &&
		k.y >= std::min(a.y, b.y) && k.y <= std::max(a.y, b.y))
		return 1;
	else return 0;
}

int n;
int ans[MAXN];

int main() {
	freopen("C-small-attempt0.in", "r", stdin);
	freopen("C1.out", "w", stdout);
	int ncas, cnt = 0;
	scanf("%d", &ncas);
	while (ncas--) {
		scanf("%d", &n);
		for (int i = 0; i < n; ++i) {
			scanf("%lld %lld", &point[i].x, &point[i].y);
		}
		if (n == 1) {
			printf("Case #%d:\n0\n", ++cnt);
		}
		for (int i = 0; i < n; ++i) {
			ans[i] = n + 1;
		}
		int top = 1 << n;
		for (int i = 0; i < top; ++i) {
			int cur = 0;
			for (int j = 0; j < n; ++j) {
				if (i & (1 << j) != 0) {
					npt[cur++] = point[j];
				}
			}
			int num = graham_scan(cur);
			for (int j = 0; j < n; ++j) {
				for (int k = 1; k < num; ++k) {
					if (on_segment(plo[k - 1], plo[k], point[j])) {
						ans[j] = std::min(ans[j], n - cur);
					}
				}
				if (on_segment(plo[0], plo[num - 1], point[j])) {
					ans[j] = std::min(ans[j], n - cur);
				}
			}
		}
		printf("Case #%d:\n", ++cnt);
		for (int i = 0; i < n; ++i) {
			printf("%d\n", ans[i]);
		}
	}
	return 0;
}