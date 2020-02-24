#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include <ctime>
#include <cassert>
#include <cstring>

using namespace std;

#define sqr(a) ((a)*(a))
#define has(mask,a) (((mask) & (1 << (a))) ? 1 : 0)

int n;

char a[1005][1005], b[1005][1005], c[1005][1005];

void load() {
	scanf ("%d\n", &n);
	n *= 2;
	for (int i = 0;i < n;i++) {
		gets (a[i]);
	}
}

int go () {
	for (int i = 0;i < n;i++) {
		for (int j = 0;j < n;j++) {
			c[i][j] = a[i][j];
		}
	}

	int ans = 0;
	for (int i = 0;i < n;i++) {
		if (b[0][i] != (a[0][i] - '0')) {
			int w = -1;
			for (int j = i + 1;j < n;j++) {
				if (b[0][j] != (a[0][j] - '0')) {
					w = j;
					break;
				}
			}
			if (w == -1) {
				//printf ("Case #%d: IMPOSSIBLE\n", test);
				return 1e9;	
			}

			ans++;
			for (int j = 0;j < n;j++) {
				swap (a[j][i], a[j][w]);
			}
		}
	}

	for (int i = 0;i < n;i++) {
		if (b[i][0] == (a[i][0] - '0')) continue;

		int w = -1;
		for (int j = i + 1;j < n;j++) {
			if (b[j][0] != (a[j][0] - '0')) {
				w = j;
				break;
			}
		}

		if (w == -1) {
			//printf ("Case #%d: IMPOSSIBLE\n", test);
			return 1e9;
		}
		ans++;
		for (int j = 0;j < n;j++) {
			swap (a[i][j], a[w][j]);
		}
	}

	for (int i = 0;i < n;i++) {
		for (int j = 0;j < n;j++) {
			if (a[i][j] - '0' != b[i][j]) return 1e9;
		}
	}

	return ans;
}


void solve (int test) {
	int one = 0;
	for (int i = 0;i < n;i++) {
		for (int j = 0;j < n;j++) {
			one += a[i][j] == '1';
		}
	}
	if (one != n * n / 2) {
		printf ("Case #%d: IMPOSSIBLE\n", test);
		return;
	}

	int ans = (int)1e9;
	for (int t = 0;t < 2;t++) {
		b[0][0] = t;
		for (int i = 0;i < n;i++) {
			for (int j = 0;j < n;j++) {
				if (j == 0) {
					if (i == 0) continue;
					b[i][j] = 1 - b[i - 1][j];
				} else {
					b[i][j] = 1 - b[i][j - 1];
				}
			}
		}
		ans = min (ans, go ());
		for (int i = 0;i < n;i++) {
			for (int j = 0;j < n;j++) {
				a[i][j] = c[i][j];
			}
		}
	}


	if (ans > 1e6) {
		printf ("Case #%d: IMPOSSIBLE\n", test);
		return;
	}
	printf ("Case #%d: %d\n", test, ans);
}

int main() {
 	freopen ("a.in", "r", stdin);

 	int t;
 	scanf ("%d\n", &t);

 	for (int i = 0;i < t;i++) {
 		load();
 		solve(i + 1);
 	}
}
