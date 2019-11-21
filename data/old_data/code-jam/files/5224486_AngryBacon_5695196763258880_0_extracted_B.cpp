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

const int MOD = (int)1e9 + 7;
const int N = 21;

int n, x;
long long binom[N][N];
long long fact[N];
long long f[N][N][N][N];

long long put3(int n, int x)
{
	return binom[n][x] * binom[n][x] % MOD * fact[x] % MOD;
}

long long solve(int n)
{
	memset(f, 0, sizeof f);
	f[0][n][0][0] = 1;
	for(int i = 0; i < n; ++ i) {
		for(int j = 0; j <= n; ++ j) { // 0
			for(int k = 0; j + k <= n; ++ k) { // 1
				for(int l = 0; l + j + k <= n; ++ l) { // 2
					if (f[i][j][k][l]) {
						//cout << f[i][j][k][l] << ' ' << i << ' ' << j << ' ' << k << ' ' << l << endl;
					}
					if (k >= 1 && l >= 1) {
						(f[i + 1][j][k - 1][l - 1] += f[i][j][k][l] * l * k) %= MOD;
					}
					if (j >= 2) {
						(f[i + 1][j - 2][k + 1][l + 1] += f[i][j][k][l] * j * (j - 1)) %= MOD;
					}
					if (j >= 1 && k >= 1) {
						(f[i + 1][j - 1][k][l] += f[i][j][k][l] * j * k) %= MOD;
					}
					if (j >= 1 && l >= 1) {
						(f[i + 1][j - 1][k][l] += f[i][j][k][l] * j * l) %= MOD;
					}
				}
			}
		} 
	}
	return f[n][0][0][0];
}

void solve()
{
	//cout << solve(2) << endl;
	//return;
	cin >> n;
	binom[0][0] = 1;
	fact[0] = 1;
	for(int i = 1; i <= n; ++ i) {
		fact[i] = fact[i - 1] * i % MOD;
		binom[i][0] = 1;
		for(int j = 1; j <= i; ++ j) {
			binom[i][j] = binom[i - 1][j - 1] + binom[i - 1][j];
			binom[i][j] %= MOD;
		}
	}
	cin >> x;
	long long ans = 0;
	for(int i = x; i <= n; ++ i) {
		ans += put3(n, i) * solve(n - i) % MOD;
	}
	cout << ans % MOD << endl;
}

int main()
{
	freopen("B-small-attempt0.in", "r", stdin); freopen("B-small-attempt0.out", "w", stdout);
	//freopen("B-small-attempt1.in", "r", stdin); freopen("B-small-attempt1.out", "w", stdout);
	//freopen("B-small-attempt2.in", "r", stdin); freopen("B-small-attempt2.out", "w", stdout);
	//freopen("B-large.in", "r", stdin); freopen("B-large.out", "w", stdout);
	int test_case;
	cin >> test_case;
	for(int i = 0; i < test_case; ++ i) {
		printf("Case #%d: ", i + 1);
		cerr << "now " << i << endl;
		solve();
	}
	return 0;
}
