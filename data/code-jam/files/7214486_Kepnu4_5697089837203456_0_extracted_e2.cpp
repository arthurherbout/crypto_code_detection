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

const int M = 10;

long long n, a, b, c[5005][5005];

int dp[1005][10];

void load() {
	cin >> n >> a >> b;
}

int go (int n, int k) {
	if (n <= 1) return 0;
	if (k == 1) return n - 1;

	int &res = dp[n][k];
	if (res != -1) return res;
	res = 1e9;

	for (int i = 1;i < n;i++) {
		res = min (res, max (go (i, k), go (n - i, k - 1)) + 1);
	}

	return res;
}

void solve (int test) {
	memset (dp, -1, sizeof (dp));

	for (int i = 0;i < 5005;i++) {
		c[0][i] = 1;
	}

	for (int i = 1;i < 5005;i++) {
		for (int j = 0;j < i;j++) {
			c[i][j] = c[i - 1][j];
		}
		for (int j = i;j < 5005;j++) {
			c[i][j] = c[i - 1][j - 1] + c[i][j - 1];
		}
	}

	/*for (int i = 0;i < 10;i++) {
		for (int j = 0;j < 10;j++) {
			cerr << c[i][j] << " ";
		}
		cerr << endl;
	}*/

	long long ans = (long long)1e16;
	for (int i = 0;i < 5005;i++) {
		for (int j = i;j < 5005;j++) {
			if (c[i][j] >= n) {
				ans = min (ans, (i - 1) * b + (j - i + 1) * a);
				break;
			}
		}
	}

	printf ("Case #%d: %I64d\n", test, ans);
}

int main() {
 	freopen ("a.in", "r", stdin);

 	int t;
 	scanf ("%d\n", &t);

 	for (int i = 0;i < t;i++) {
 		cerr << i << endl;
 		load();
 		solve(i + 1);
 	}
}
