#include <climits>
#include <cstring>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;

#define foreach(iter, cont) \
	for (typeof((cont).begin()) iter = (cont).begin(); iter != (cont).end(); iter++)

#define MOD 100003
int C[501][501];

int f[501][501];
int bf (int a, int k)
{
	assert (k < a);
	if (k == 1)
		return 1;
	if (f[a][k] > 0)
		return f[a][k];

	int res = 0;
	for (int k1 = k-1; k1 >= 1; k1--) {
		int c = k - k1 - 1;
		int n = a - k - 1;
		if (c > n)
			break;
		long long out = bf (k, k1);
		out *= C[n][c];
		res += (out % MOD);
		res %= MOD;
	}
	return f[a][k] = res;
}

int solve (int n)
{
	int res = 0;
	for (int i = 0; i <= 500; i++)
	for (int j = 0; j <= 500; j++)
		f[i][j] = -1;

	for (int k = 1; k < n; k++) {
		//printf ("%d\n", bf (n, k));
		res += bf (n, k);
		res %= MOD;
	}
	return res;
}

int main ()
{
	C[0][0] = 1;
	for (int n = 1; n <= 500; n++) {
		C[n][0] = 1;
		C[n][n] = 1;
		for (int k = 1; k < n; k++) {
			C[n][k] = C[n-1][k-1] + C[n-1][k];
			C[n][k] %= MOD;
		}
	}
	int T;
	scanf ("%d", &T);
	for (int i = 1; i <= T; i++) {
		int n;
		scanf ("%d", &n);
		printf ("Case #%d: %d\n", i, solve (n));
	}
	return 0;
}
