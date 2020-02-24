#include <cmath>
#include <cstdio>
#include <cctype>
#include <cstdlib>
#include <climits>
#include <cstring>
#include <vector>
#include <string>
#include <iostream>
#include <cassert>
#include <algorithm>

using namespace std;

#define foreach(e,x) for(__typeof((x).begin()) e=(x).begin(); e!=(x).end(); ++e)

const int N = 10000 + 10;
const int M = 10;

int n;
char a[N];
int s[N];
int toL[M][N];
int toR[M][N];
int ptr[M];

long long solve(int l, int r)
{
	if (l > r) return 0;
	if (l == r) {
		return a[l] - '0';
	}
	int p = l, q = r, mid;
	for( ; p + 1 < q; ) {
		mid = (p + q) >> 1;
		int sl = s[mid - 1] - s[l - 1];
		int sr = s[r] - s[mid];
		if (sl <= sr) {
			p = mid;
		} else {
			q = mid;
		}
	}
	int ans0 = abs((s[p - 1] - s[l - 1]) - (s[r] - s[p]));
	int ans1 = abs((s[p] - s[l - 1]) - (s[r] - s[p + 1]));
	int choose;
	if (ans0 <= ans1) {
		choose = p;
	} else {
		choose = p + 1;
	}
	return a[choose] - '0' + max(solve(l, choose - 1), solve(choose + 1, r));
}

/*
int f[N][N];

long long bf()
{
	for(int i = n; i >= 1; -- i) {
		for(int j = i; j <= n; ++ j) {
			f[i][j] = 1000000;
			for(int k = i; k <= j; ++ k) {
				f[i][j] = min(f[i][j], a[k] - '0' + max(f[i][k - 1], f[k + 1][j]));
			}
		}
	}
	return f[1][n];
}
*/

void updateMin(int &a, int b)
{
	if (b < 0) return;
	if (a < 0) {
		a = b;
	} else if (a > b) {
		a = b;
	}
}

void updateMax(int &a, int b)
{
	if (b < 0) return;
	if (a < 0) {
		a = b;
	} else if (a < b) {
		a = b;
	}
}

void solve()
{
	scanf("%s", a + 1);
	n = strlen(a + 1);	
	for(int i = 1; i <= n; ++ i) {
		s[i] = s[i - 1] + a[i] - '0';
	}
	int lim = solve(1, n);

	memset(toL, -1, sizeof toL);
	memset(toR, -1, sizeof toR);
	for(int i = 1; i <= lim; ++ i) {
		int cur = i % 10;
		for(int j = 1; j <= n; ++ j) {
			toL[cur][j] = toL[(cur + 9) % 10][j];
		}
		for(int j = 1; j < 10; ++ j) {
			ptr[j] = 0;
		}
		for(int j = 1; j <= n; ++ j) {
			for(int k = 1; k < 10; ++ k) {
				int tmp = (i - k + 10) % 10;
				for( ; ptr[k] < j && (toR[tmp][ptr[k]] < 0 || toR[tmp][ptr[k]] >= 0 && toR[tmp][ptr[k]] < j - 1); ++ ptr[k]);
			}
			if (i >= a[j] - '0') {
				updateMin(toL[cur][j], j);
			}
			int tmp = (i - (a[j] - '0') + 10) % 10;
			updateMin(toL[cur][j], toL[tmp][ptr[a[j] - '0']]);
		}

		for(int j = 1; j <= n; ++ j) {
			toR[cur][j] = toR[(cur + 9) % 10][j];
		}
		for(int j = 1; j < 10; ++ j) {
			ptr[j] = n + 1;
		}
		for(int j = n; j >= 1; -- j) {
			for(int k = 1; k < 10; ++ k) {
				int tmp = (i - k + 10) % 10;
				for( ; ptr[k] > j && (toL[tmp][ptr[k]] < 0 || toL[tmp][ptr[k]] >= 0 && toL[tmp][ptr[k]] > j + 1); -- ptr[k]);
			}
			if (i >= a[j] - '0') {
				updateMax(toR[cur][j], j);
			}
			int tmp = (i - (a[j] - '0') + 10) % 10;
			updateMax(toR[cur][j], toR[tmp][ptr[a[j] - '0']]);
		}
		for(int j = 1; j <= n; ++ j) {
			if (toL[cur][j] == 1 && toR[cur][j] == n) {
				cout << i << endl;
				return;
			}
		}
	}
}

int main()
{
	//freopen("A-small-attempt0.in", "r", stdin); freopen("A-small-attempt0.out", "w", stdout);
	freopen("A-small-attempt1.in", "r", stdin); freopen("A-small-attempt1.out", "w", stdout);
	//freopen("A-small-attempt2.in", "r", stdin); freopen("A-small-attempt2.out", "w", stdout);
	//freopen("A-large.in", "r", stdin); freopen("A-large.out", "w", stdout);
	int test_case;
	cin >> test_case;
	for(int i = 0; i < test_case; ++ i) {
		printf("Case #%d: ", i + 1);
		cerr << "now " << i << endl;
		solve();
	}
	return 0;
}
