#include <cstdio>
#include <algorithm>
#include <vector>
#include <queue>
#include <stack>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <string>
#include <set>
#include <map>
#include <thread>
#include <mutex>
#include <cassert>

using namespace std;
#define MOD @
#define ADD(X,Y) ((X) = ((X) + (Y)%MOD) % MOD)
typedef long long i64; typedef vector<int> ivec; typedef vector<string> svec;

const int MAXN = 160;
const int MAXD = 13000;

int nTC, tcid;
int N;
i64 X[MAXN], Y[MAXN], Z[MAXN];
int dist[MAXN][MAXN];

i64 myabs(i64 a)
{
	return a > 0 ? a : -a;
}
int nnom(int a, int b)
{
	return myabs(X[a] - X[b]) + myabs(Y[a] - Y[b]) + myabs(Z[a] - Z[b]);
}

int dp[MAXN][MAXD + 10];
set<int> unvis[MAXN];
queue<pair<int, int> > Q;

void appd(int p, int d, int v)
{
	dp[p][d] = v;
	unvis[p].erase(d);
	Q.push({p, d});
}
int main()
{
	scanf("%d", &nTC);
	tcid = 0;
	
	for (; tcid++ < nTC; ) {
		scanf("%d", &N);
		N += 2;
		for (int i = 0; i < N; ++i) {
			scanf("%lld%lld%lld", X + i, Y + i, Z + i);
		}
		int ans;

		if (nnom(0, 1) % 2 != 0) goto fail;
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < N; ++j) {
				dist[i][j] = nnom(i, j);
			}
		}

		for (int i = 0; i < N; ++i) {
			unvis[i].clear();
			for (int j = 0; j <= MAXD; ++j) {
				dp[i][j] = 100000000;
				if (dist[0][i] % 2 == j % 2) {
					unvis[i].insert(j);
				}
			}
		}

		for (int i = 2; i < N; ++i) {
			appd(i, nnom(0, i), 0);
		}

		while (!Q.empty()) {
			auto t = Q.front(); Q.pop();
			int p = t.first, d = t.second;
			//printf("%d %d %d\n", p, d, dp[p][d]);

			if (p < 2) continue;
			for (int q = 2; q < N; ++q) {
				int lo = min(MAXD, abs(d - dist[p][q]));
				int hi = min(MAXD, d + dist[p][q]);

				for (auto iter = unvis[q].lower_bound(lo); iter != unvis[q].end() && *iter <= hi;) {
					assert(lo <= *iter && *iter <= hi);
					auto itt = iter;
					++itt;
					appd(q, *iter, dp[p][d] + 1);
					iter = itt;
				}
			}
		}
		ans = 100000000;
		for (int i = 2; i < N; ++i) {
			ans = min(ans, 1 + dp[i][dist[1][i]]);
		}
		if (ans > 100000) {
			goto fail;
		}
		printf("Case #%d: %d\n", tcid, ans);
		continue;

	fail:
		printf("Case #%d: IMPOSSIBLE\n", tcid);
	}
	
	return 0;
}

