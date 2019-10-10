//#pragma comment(linker, "/STACK:134217728")

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

int T[1 << 10];
int A[1 << 10];
int B[1 << 10];
int C[1 << 10];
int E[1 << 10][1 << 10];
int Mark[1 << 10];
int gN;

int dfs(int a)
{
	if (a == gN - 1)
		return 1;

	Mark[a] = 1;
	int i;
	FOR(i, 0, gN)
	if (Mark[i] == 0 && E[a][i] > 0 && dfs(i) != 0)
	{
		--E[a][i];
		++E[i][a];
		return 1;
	}

	return 0;
}

int SolveTest(int test)
{
	int N;
	scanf("%d", &N);

	CLEAR(B, -1);

	int i, j, k;
	vector< pair<int, PII> > v;
	FOR(i, 0, N)
	{
		char buf[7];
		int a;
		scanf("%s%d", buf, &a);

		int type = buf[0] == 'E' ? 0 : 1;

		if (a == 0)
		{
			B[i] = type;
		}
		else
		{
			v.push_back(MP(a, PII(i, type)));
		}
	}

	CLEAR(C, -1);
	sort(ALL(v));

	int prev = 0;
	int all = 0;
	CLEAR(E, 0);
	FOR(i, 1, SIZE(v) + 1)
	if (i == SIZE(v) || v[i].first != v[prev].first)
	{
		int state = v[prev].second.second ^ 1;
		FOR(j, prev, i)
		{
			int s = v[j].second.second;
			if (s == state)
			{
				int from = v[j - 1].second.first;
				int to = v[j].second.first;
				E[N][from] = 1;
				++all;
				FOR(k, from, to)
				if (B[k] == (state ^ 1))
					E[from][k] = 1;
			}
			else
				state = s;
		}

		prev = i;
	}

	FOR(i, 0, N)
	if (B[i] != -1)
		E[i][N + 1] = 1;

	gN = N + 2;
	int flow = 0;
	while (true)
	{
		CLEAR(Mark, 0);
		if (dfs(N) == 0)
			break;

		++flow;
	}

	if (flow != all)
	{
		printf("Case #%d: CRIME TIME\n", test + 1);
		return 0;
	}

	FOR(i, 0, N)
	if (B[i] != -1 && E[N + 1][i] == 0)
		C[i] = B[i];

	CLEAR(E, 0);
	int total = 0;
	FOR(i, 0, N)
	if (C[i] == 0)
	{
		++total;
		E[N][i] = 1;
	}
	else if (C[i] == 1)
	{
		E[i][N + 1] = 1;
	}

	prev = 0;
	FOR(i, 1, SIZE(v) + 1)
	if (i == SIZE(v) || v[i].first != v[prev].first)
	{
		if (v[prev].second.second == 1)
		{
			FOR(j, 0, v[prev].second.first)
			if (C[j] == 0)
				++E[j][v[prev].second.first] = 1;

			E[v[prev].second.first][N + 1] = 1;
		}

		if (v[i - 1].second.second == 0)
		{
			FOR(j, v[i - 1].second.first + 1, N)
			if (C[j] == 1)
				E[v[i - 1].second.first][j] = 1;

			E[N][v[i - 1].second.first] = 1;
			++total;
		}

		prev = i;
	}

	flow = 0;
	while (true)
	{
		CLEAR(Mark, 0);
		if (dfs(N) == 0)
			break;

		++flow;
	}

	printf("Case #%d: %d\n", test + 1, total - flow);
	return 0;
}

int main()
{
	freopen("c.in", "r", stdin);
	freopen("c.out", "w", stdout);

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
