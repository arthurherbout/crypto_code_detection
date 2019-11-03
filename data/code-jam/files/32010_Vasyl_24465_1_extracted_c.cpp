#include <iostream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <memory>
#include <cctype>
#include <string>
#include <vector>
#include <list>
#include <queue>
#include <deque>
#include <stack>
#include <map>
#include <set>
#include <algorithm>
using namespace std;

typedef long long Int;
typedef pair<int,int> PII;
typedef vector<int> VInt;

#define FOR(i, a, b) for(i = (a); i < (b); ++i)
#define RFOR(i, a, b) for(i = (a) - 1; i >= (b); --i)
#define CLEAR(a, b) memset(a, b, sizeof(a))
#define SIZE(a) int((a).size())
#define ALL(a) (a).begin(),(a).end()
#define PB push_back
#define MP make_pair

#define MOD 1000000009

int P[512];
Int Res[512];
VInt E[512];
int C[512];
int K;

Int dfs(int a)
{
	int cnt = K;
	if(P[a] != -1)
		cnt -= C[ P[a] ];

	Int& res = Res[a];
	res = 1;

	int i;
	FOR(i, 0, C[a])
	{
		int b = E[a][i];
		if(b == P[a])
			continue;

		P[b] = a;
		res *= (cnt*dfs(b) % MOD);
		res %= MOD;
		--cnt;
	}

	return res;
}

int SolveTest(int test)
{
	int N;
	scanf("%d%d", &N, &K);

	int i;
	FOR(i, 0, N - 1)
	{
		int a, b;
		scanf("%d%d", &a, &b);
		--a;
		--b;

		E[a].PB(b);
		E[b].PB(a);
	}

	FOR(i, 0, N)
		C[i] = SIZE(E[i]);

	CLEAR(P, -1);
	printf("Case #%d: %d\n", test + 1, int(dfs(0)));

	FOR(i, 0, N)
		E[i].clear();

	return 0;
}

int main()
{
	freopen("c.in", "r", stdin);
	freopen("c.out", "w", stdout);

	char buf[1 << 10];
	gets(buf);
	int T, t;
	sscanf(buf, "%d", &T);
	FOR(t, 0, T)
	{
		fprintf(stderr, "Test %d of %d\n", t + 1, T);
		SolveTest(t);
	}

	return 0;
};
