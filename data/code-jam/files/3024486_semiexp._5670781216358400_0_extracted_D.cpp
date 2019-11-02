#include <cstdio>
#include <algorithm>
#include <vector>
#include <queue>
#include <stack>
#include <set>
#include <map>
#include <cmath>
#include <string>
#include <cstring>
using namespace std;
typedef long long i64;

#define INF 1000000000

int N, C[4050];
vector<int> con[4050];

int p0, q0;
int dp[2][85][85];
int dist[85][85];

int onway(int p, int q, int r)
{
	return dist[p][q] == dist[p][r] + dist[r][q];
}

int calc(int p, int q, int t, int curpt)
{
	if(dp[t][p][q] == -INF*2 || dp[t][p][q] == INF*2) return curpt;

	if(dp[t][p][q] != INF) return dp[t][p][q];

	if(t == 0) {
		dp[t][p][q] = -INF * 2;

		for(int i = 0; i < con[p].size(); i++) {
			int p2 = con[p][i];

			if(onway(p0, p, p2) || (onway(q0, q, p2) && onway(q0, q, p))) continue;

			int c2 = curpt;
			if(!onway(q0, q, p2)) c2 += C[p2];

			dp[t][p][q] = max(dp[t][p][q], calc(p2, q, 1-t, c2));
		}

		if(dp[t][p][q] == -INF * 2) {
			dp[t][p][q] = calc(p, q, 1-t, curpt); //curpt;
		}

	} else {
		dp[t][p][q] = INF * 2;

		for(int i = 0; i < con[q].size(); i++) {
			int q2 = con[q][i];

			if(onway(q0, q, q2) || (onway(p0, p, q2) && onway(p0, p, q))) continue;

			int c2 = curpt;
			if(!onway(p0, p, q2)) c2 -= C[q2];

			dp[t][p][q] = min(dp[t][p][q], calc(p, q2, 1-t, c2));
		}

		if(dp[t][p][q] == INF * 2) {
			dp[t][p][q] = calc(p, q, 1-t, curpt); //curpt;
		}
	}
	//if(p0==7&&q0==0) printf("%d %d %d: %d\n", t, p, q, dp[t][p][q]);
	return dp[t][p][q];
}

int solve(int p_, int q_)
{
	for(int i=0;i<N;i++) for(int j=0;j<N;j++) dp[0][i][j] = dp[1][i][j] = INF;
	p0 = p_; q0 = q_;

	int c = C[p0];
	if(p0!=q0) c -= C[q0];

	int ret = calc(p0, q0, 0, c);
	//printf("%d %d: %d\n", p0, q0, ret);
	return ret;
}

int main()
{
	int T;
	scanf("%d", &T);

	for(int t = 0; t++ < T; ) {
		scanf("%d", &N);
		for(int i=0;i<N;i++) con[i].clear();

		for(int i = 0; i < N; i++) scanf("%d", C+i);
		for(int i=0;i<N;i++) for(int j=0;j<N;j++) dist[i][j] = i==j ? 0 : N+1;
		for(int i = 0; i < N-1; i++) {
			int p;
			scanf("%d", &p);
			--p;

			con[i].push_back(p);
			con[p].push_back(i);

			dist[i][p] = dist[p][i] = 1;
		}
		for(int i=0;i<N;i++) for(int j=0;j<N;j++) for(int k=0;k<N;k++) dist[j][k] = min(dist[j][k], dist[j][i] + dist[i][k]);

		int ret = -N * 10000;
		for(int i = 0; i < N; i++) {
			int tmp = N * 10000;
			for(int j = 0; j < N; j++) {
				int sol = solve(i, j);

				tmp = min(tmp, sol);
			}

			ret = max(ret, tmp);
		}

		printf("Case #%d: %d\n", t, ret);
	}

	return 0;
}
