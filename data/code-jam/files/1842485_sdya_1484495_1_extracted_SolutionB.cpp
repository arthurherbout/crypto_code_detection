#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <bitset>
#include <cstring>
#include <string>
#include <queue>
#include <stack>
#include <set>
#include <map>
#include <ctime>
#pragma comment (linker, "/STACK:256000000")

using namespace std;

const int maxN = 10000;
int n, w, l;
int a[maxN];

int x[maxN], y[maxN];

bool check(int n) {
	if (x[n] < 0 || x[n] > w) {
			return false;
	}
	if (y[n] < 0 || y[n] > l) {
		return false;
	}
	for (int i = 1; i < n; ++i) {
		for (int j = n; j <= n; ++j) {
			long long dx = x[j] - x[i];
			long long dy = y[j] - y[i];
			if (dx < 0) dx = - dx;
			if (dy < 0) dy = - dy;
			if (dx < a[i] + a[j] && dy < a[i] + a[j]) {
				return false;
			}
		}
	}
	return true;
}

bool good = false;

int used[maxN];
int p[maxN];

void rec(int have, int test) {
	if (good) return ;
	if (!check(have)) return ;
	if (have == n) {
		good = true;
		vector < int > nx(n + 1), ny(n + 1);
		for (int i = 1; i <= n; ++i) {
			nx[p[i]] = x[i], ny[p[i]] = y[i];
		}
		printf("Case #%d:", test);
		for (int i = 1; i <= n; ++i) {
			printf(" %d %d", nx[i], ny[i]);
		}
		printf("\n");
		return ;
	}

	x[have + 1] = 0, y[have + 1] = 0;
	rec(have + 1, test);
	for (int i = 0; i <= have; ++i) {
		x[have + 1] = x[i] + a[i] + a[have + 1], y[have + 1] = y[i];
		rec(have + 1, test);
		x[have + 1] = x[i] + a[i] + a[have + 1], y[have + 1] = y[i] + a[i];
		rec(have + 1, test);
		x[have + 1] = x[i], y[have + 1] = y[i] + a[i] + a[have + 1];
		rec(have + 1, test);
		x[have + 1] = x[i] + a[i], y[have + 1] = y[i] + a[i] + a[have + 1];
		rec(have + 1, test);
	}
}

void solve(int lx, int ly, int rx, int ry) {
	if (lx > rx || ly > ry) {
		return ;
	}
	for (int i = 1; i <= n; ++i) {
		if (!used[i]) {
			x[i] = a[i] + lx, y[i] = a[i] + ly;
			if (lx == 0) {
				x[i] -= a[i];
			}
			if (ly == 0) {
				y[i] -= a[i];
			}
			if (x[i] < lx || x[i] > rx) continue;
			if (y[i] < ly || y[i] > ry) continue;
			used[i] = true;
			if (rx - x[i] - a[i] - lx >= ry - y[i] - a[i] - ly) {
				solve(x[i] + a[i] + lx, ly, rx, ry);
				solve(lx, y[i] + a[i] + ly, min(rx, x[i] + a[i] + lx), ry);
			} else {
				solve(lx, y[i] + a[i] + ly, min(rx, x[i] + a[i] + lx), ry);
				solve(x[i] + a[i] + lx, ly, rx, ry);
			}
			return ;
		}
	}
}

struct board {
	int lx, ly, rx, ry;
	board() {
	}
	board(int lx, int ly, int rx, int ry) : lx(lx), ly(ly), rx(rx), ry(ry) {
	}
	long long area() const {
		long long dx = rx - lx;
		long long dy = ry - ly;
		long long s = dx * dy;
		if (s < 0) s = -s;
		return s;
	}
	bool operator < (const board &other) const {
		return area() > other.area();
	}
};

void solve(int test) {
	scanf("%d%d%d", &n, &w, &l);
	for (int i = 1; i <= n; ++i) {
		scanf("%d", &a[i]), p[i] = i;
	}
	for (int i = 1; i <= n; ++i) {
		for (int j = i + 1; j <= n; ++j) {
			if (a[j] > a[i]) {
				swap(a[j], a[i]);
				swap(p[j], p[i]);
			}
		}
	}

	good = false;
	/*for (int i = 1; i <= n; ++i) {
		used[i] = false;
	}

	set < board > S;
	S.insert(board(0, 0, w, l));
	while (!S.empty()) {
		board cur = * S.begin(); S.erase(S.begin());
		int lx, ly, rx, ry;
		lx = cur.lx;
		ly = cur.ly;
		rx = cur.rx;
		ry = cur.ry;
		if (lx > rx || ly > ry) {
			continue;
		}
		for (int i = 1; i <= n; ++i) {
			if (!used[i]) {
				x[i] = a[i] + lx, y[i] = a[i] + ly;
				if (lx == 0) {
					x[i] -= a[i];
				}
				if (ly == 0) {
					y[i] -= a[i];
				}
				if (x[i] < lx || x[i] > rx) continue;
				if (y[i] < ly || y[i] > ry) continue;
				used[i] = true;
				S.insert(board(x[i] + a[i] + lx, ly, rx, ry));
				S.insert(board(lx, y[i] + a[i] + ly, min(rx, x[i] + a[i] + lx), ry));
				break;
			}
		}
	}
	//solve(0, 0, w, l);

	if (!check()) {
		cerr << "BAD" << endl;
	}

	vector < int > nx(n + 1), ny(n + 1);
	for (int i = 1; i <= n; ++i) {
		nx[p[i]] = x[i], ny[p[i]] = y[i];
	}
	printf("Case #%d:", test);
	for (int i = 1; i <= n; ++i) {
		printf(" %d %d", nx[i], ny[i]);
	}
	printf("\n");*/
	rec(0, test);
}

int main() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	int tests;
	scanf("%d", &tests);
	for (int i = 0; i < tests; ++i) {
		solve(i + 1);
		cerr << i << endl;
	}
	return 0;
}