#pragma comment(linker, "/STACK:134217728")

#include <iostream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cctype>
#include <cstring>
#include <vector>
#include <list>
#include <queue>
#include <deque>
#include <stack>
#include <map>
#include <set>
#include <algorithm>
#include <numeric>
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

Int S[1 << 20];
Int M[1 << 20];
VInt E[1 << 20];
int R[1 << 20];
int gFrom, gTo, gD;

void generate(int N, Int A[], Int a0, Int a, Int c, Int r)
{
	int i;
	FOR(i, 0, N)
	{
		A[i] = a0;
		a0 = (a0 * a + c) % r;
	}
}

void dfs(int a, int Min, int Max)
{
	Min = min(Min, int(S[a]));
	Max = max(Max, int(S[a]));
	int i;
	FOR(i, 0, SIZE(E[a]))
	{
		int b = E[a][i];
		dfs(b, Min, Max);
	}

	int from = max(0, Max - gD);
	int to = Min;

	if (from <= to)
	{
		++R[from];
		--R[to + 1];
	}
}

int SolveTest(int test)
{
	int N, D;
	scanf("%d%d", &N, &D);

	int S0, As, Cs, Rs;
	scanf("%d%d%d%d", &S0, &As, &Cs, &Rs);

	int M0, Am, Cm, Rm;
	scanf("%d%d%d%d", &M0, &Am, &Cm, &Rm);

	generate(N, S, S0, As, Cs, Rs);
	generate(N, M, M0, Am, Cm, Rm);

	int i;
	FOR(i, 1, N)
	{
		int p = M[i] % i;
		E[p].push_back(i);
	}

	CLEAR(R, 0);
	int s = (int)S[0];
	gFrom = max(0, s - D);
	gTo = s;
	gD = D;
	dfs(0, s, s);

	int sum = 0;
	int res = 0;
	FOR(i, 0, 1 << 20)
	{
		sum += R[i];
		res = max(res, sum);
	}

	FOR(i, 0, N)
		E[i].clear();

	printf("Case #%d: %d\n", test + 1, res);
	return 0;
}

int main()
{
	freopen("a.in", "r", stdin);
	freopen("a.out", "w", stdout);

	int T, t;
	char buf[1 << 7];
	gets(buf);
	sscanf(buf, "%d", &T);
	FOR(t, 0, T)
	{
		fprintf(stderr, "Solving %d/%d\n", t + 1, T);
		SolveTest(t);
	}

	return 0;
};
