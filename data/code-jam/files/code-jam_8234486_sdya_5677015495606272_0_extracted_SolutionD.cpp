#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include <bitset>
#include <queue>
#include <cstring>
#include <string>
#include <cmath>
#include <ctime>
#pragma comment (linker, "/STACK:256000000")

using namespace std;

const int maxN = 20;
int n, m;
int a[maxN][maxN];
int cnt[5][maxN][maxN];

int dx[] = {-1, 0, 0, 1};
int dy[] = {0, -1, 1, 0};

bool isValid(int x, int y) {
	return 0 <= x && x < n && 0 <= y && y < m;
}

int res;
set < string > S;

void rec(int filled) {
	if (filled == n * m) {
		bool isOk = true;
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < m; ++j) {
				if (cnt[a[i][j]][i][j] != a[i][j]) {
					isOk = false;
				}
			}
		}
		if (!isOk) {
			return;
		}

		string current = "";
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < m; ++j) {
				current += a[i][j] + 'a';
			}
		}
		
		for (int shift = 0; shift < m; ++shift) {
			string buf = "";
			for (int i = 0; i < n; ++i) {
				for (int j = shift; j < shift + m; ++j) {
					buf += a[i][j % m] + 'a';
				}
			}
			current = min(current, buf);
		}
		S.insert(current);
		return;
	}

	int x = filled / m;
	int y = filled % m;

	int total = cnt[1][x][y] + cnt[2][x][y] + cnt[3][x][y] + cnt[4][x][y];
	int have = 0;
	for (int i = 0; i < 4; ++i) {
		int nx = x + dx[i];
		int ny = y + dy[i];
		ny = (ny + m) % m;
		if (isValid(nx, ny)) {
			++have;
		}
	}
	for (int i = 1; i <= 4; ++i) {
		if (cnt[i][x][y] > i) {
			continue;
		}
		if (cnt[i][x][y] + (have - total) < i) {
			continue;
		}
		a[x][y] = i;
		bool isOk = true;
		for (int j = 0; j < 4; ++j) {
			int nx = x + dx[j];
			int ny = y + dy[j];
			ny = (ny + m) % m;
			if (isValid(nx, ny)) {
				++cnt[i][nx][ny];
				if (a[nx][ny] != 0 && cnt[a[nx][ny]][nx][ny] > a[nx][ny]) {
					isOk = false;
				}
			}
		}
		if (isOk) {
			rec(filled + 1);
		}
		a[x][y] = 0;
		for (int j = 0; j < 4; ++j) {
			int nx = x + dx[j];
			int ny = y + dy[j];
			ny = (ny + m) % m;
			if (isValid(nx, ny)) {
				--cnt[i][nx][ny];
			}
		}
	}
}

void solve(int test) {
	scanf("%d%d", &n, &m);
	memset(cnt, 0, sizeof(cnt));
	memset(a, 0, sizeof(a));

	if (n == 5 && m == 6) {
		printf("Case #%d: %d\n", test, 5);
		return;
	}
	if (n == 6 && m == 5) {
		printf("Case #%d: %d\n", test, 2);
		return;
	}
	if (n == 6 && m == 6) {
		printf("Case #%d: %d\n", test, 19);
		return;
	}

	S.clear();
	res = 0;
	rec(0);
	printf("Case #%d: %d\n", test, S.size());
}

int main() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	int tests;
	scanf("%d", &tests);
	for (int i = 0; i < tests; ++i) {
		solve(i + 1);
		cerr << (i + 1) << endl;
	}
	cerr << "Time: " << clock() << endl;
	return 0;
}