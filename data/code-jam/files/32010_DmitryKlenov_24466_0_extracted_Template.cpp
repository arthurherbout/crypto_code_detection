#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <cassert>
#include <cmath>
#include <queue>
#include <algorithm>
#include <string>
#include <utility>
#include <sstream>

using namespace std;

#define forn(i, n) for(int i = 0; i < (int)(n); ++i)
#define fs first
#define sc second
#define pb push_back
#define mp make_pair
typedef long long int64;

const double EPS = 1E-8;

const int MOD = 30031;

int d[2001][1 << 10];
int n, k, p;

int bit[20];

int main() {
	freopen("input.txt", "rt", stdin);
	freopen("output.txt", "wt", stdout);

	bit[0] = 1;
	for(int i = 1; i < 20; ++i)
		bit[i] = bit[i - 1] << 1;

	int tk;
	scanf("%d\n", &tk);
	for(int tc = 1; tc <= tk; ++tc) {
		printf("Case #%d: ", tc);
		memset(d, 0, sizeof d);
		cin >> n >> k >> p;
		if (k > p) {
			puts("0");
			continue;
		}
		d[0][(1 << k) - 1] = 1;
		for(int i = 0; i < n-k; ++i)
			forn(msk, 1 << p) {
				if (!(msk & bit[0])) continue;
				int nm = msk >> 1;
				for(int j = 0; j < p; ++j)
					if ((nm & bit[j]) == 0)
						d[i + 1][nm | bit[j]] = (d[i + 1][nm | bit[j]] + d[i][msk]) % MOD;
			}

		printf("%d\n", d[n-k][(1 << k) - 1]);
	}

	return 0;
}