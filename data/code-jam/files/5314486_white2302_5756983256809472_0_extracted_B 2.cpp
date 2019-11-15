/*AMETHYSTS*/
#pragma comment(linker, "/STACK:1000000000")
#include <cstdio>
#include <iostream>
#include <ctime>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstring>
#include <set>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <bitset>
#include <fstream>
#include <deque>
#include <stack>
#include <climits>
#include <string>
#include <queue>
#include <memory.h>
#include <map>
#include <unordered_map>

#define ll long long
#define ld double
#define pii pair <int, int>
#define pll pair <ll, ll>
#define mp make_pair

using namespace std;

const int maxn = 33;

int n, m;
int st;
char c[maxn][maxn];
vector <pii> s;
vector <pii> tr;
int dp[1 << 10][1 << 10];
pii lnk[1 << 10][1 << 10];
int num[maxn][maxn];

int dx[4] = {-1, 0, 1, 0};
int dy[4] = {0, -1, 0, 1};
int d[maxn][maxn];

int go(int m1, int m2) {
	if (dp[m1][m2] != -1) {
		return dp[m1][m2];
	}

	if (m1 == (1 << (int)s.size()) - 1 || m2 == (1 << (int)tr.size()) - 1) {
		return dp[m1][m2] = 0;
	}

	int cn[maxn][maxn];
	int bit[maxn][maxn];

	memset(cn, 0, sizeof cn);
	memset(bit, 0, sizeof bit);

	for (int i = 0; i < (int)tr.size(); i++) {
		if ((m2 >> i) & 1) {
			continue;
		}

		int x = tr[i].first;
		int y = tr[i].second;

		for (int it = 0; it < 4; it++) {
			int nx = x;
			int ny = y;

			while (0 <= nx && nx < n && 0 <= ny && ny < m && (c[nx][ny] == '.' || c[nx][ny] == 'S' || c[nx][ny] == 'T')) {
				cn[nx][ny] = 1;
				bit[nx][ny] |= (1 << i);
				nx += dx[it];
				ny += dy[it];
			}
		}
	}

	int ans = 0;

	for (int i = 0; i < (int)s.size(); i++) {
		if ((m1 >> i) & 1) {
			continue;
		}

		queue <pii> q;
		memset(d, -1, sizeof d);

		int x = s[i].first;
		int y = s[i].second;

		q.push(mp(x, y));
		d[x][y] = 0;

		int bt = 0;

		while (!q.empty()) {
			x = q.front().first;
			y = q.front().second;
			bt |= bit[x][y];
			q.pop();

			if (d[x][y] == st) {
				continue;
			}

			for (int it = 0; it < 4; it++) {
				if (cn[x][y] == 1) {
					continue;
				}

				int nx = x + dx[it];
				int ny = y + dy[it];

				if (0 <= nx && nx < n && 0 <= ny && ny < m && d[nx][ny] == -1 && (c[nx][ny] == '.' || c[nx][ny] == 'S' || (c[nx][ny] == 'T' && ((m2 >> num[nx][ny]) & 1)))) {
					d[nx][ny] = d[x][y] + 1;
					q.push(mp(nx, ny));
				}
			}
		}

		for (int j = 0; j < (int)tr.size(); j++) {
			if ((bt >> j) & 1) {
				int res = go(m1 | (1 << i), m2 | (1 << j)) + 1;

				if (res > ans) {
					ans = res;
					lnk[m1][m2] = mp(i, j);
				}
			}
		}
	}

	return dp[m1][m2] = ans;
}


int main() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
	int t;

	cin >> t;

	for (int itt = 1; itt <= t; itt++) {
		cerr << itt << endl;
		printf("Case #%d: ", itt);

		s.clear();
		tr.clear();

		cin >> m >> n >> st;

		memset(num, 0, sizeof num);

		for (int i = 0; i < n; i++) {
			scanf("%s", c[i]);

			for (int j = 0; j < m; j++) {
				if (c[i][j] == 'S') {
					s.push_back(mp(i, j));
				} else if (c[i][j] == 'T') {
					tr.push_back(mp(i, j));
					num[i][j] = (int)tr.size() - 1;
				}
			}
		}

		memset(dp, -1, sizeof dp);

		cout << go(0, 0) << endl;

		int m1 = 0;
		int m2 = 0;

		while (dp[m1][m2] != 0) {
			int a = lnk[m1][m2].first;
			int b = lnk[m1][m2].second;

			printf("%d %d\n", a + 1, b + 1);

			m1 |= (1 << a);
			m2 |= (1 << b);
		}
	}

	return 0;
}
