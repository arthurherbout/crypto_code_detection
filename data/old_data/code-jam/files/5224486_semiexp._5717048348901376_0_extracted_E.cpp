#include <cstdio>
#include <algorithm>
#include <vector>
#include <map>
#include <queue>
#include <string>
#include <stack>
#include <cstring>
#include <cstdlib>
#include <cmath>

using namespace std;
typedef long long i64;
#define MOD @
#define ADD(X,Y) ((X) = ((X) + (Y)) % MOD)
#define DBG(...) (fprintf(stderr, "[line %d]: ", __LINE__), fprintf(stderr, __VA_ARGS__))

int N, M, S[150][10];
int P[10];

int solve(int a)
{
	int V[10]; for(int i = 0; i < M; ++i) V[i] = 0;
	vector<pair<int, int> > seq;
	for (int i = 0; i < N; ++i) {
		int mask = 0;
		if (M == 1) {
			mask = S[i][0];
		} else {
			int u = S[i][0], v = S[i][1];
			if (a) swap(u, v);
			
			if (u == 0) u = v;
			if (v == 0) v = u;
			
			mask = (u > 0 ? 2 : 0) | (v > 0 ? 1 : 0);
		}
		seq.push_back(make_pair(mask, i));
	}
	sort(seq.begin(), seq.end());
	for (auto a : seq) {
		int i = a.second;
		for (int j = 0; j < M; ++j) {
			V[j] = max(0, V[j] + S[i][j]);
		}
	}
	
	int ret = 0;
	for (int i =0; i < M; ++i) ret += V[i];
	return ret;
}

int main()
{
	int n_testcase;
	scanf("%d", &n_testcase);
	
	for (int testcase_id = 0; testcase_id++ < n_testcase; ) {
		// input and solve the problem
		scanf("%d%d", &N, &M);
		for (int i = 0; i < N; ++i) for (int j = 0; j < M; ++j) scanf("%d", &(S[i][j]));
		for (int i = 0; i < M; ++i) P[i] = i;
		int ret = 0;
		ret = solve(0);
		ret = max(ret, solve(1));

		printf("Case #%d: %d\n", testcase_id, ret);
		// print answer here
	}
	return 0;
}

