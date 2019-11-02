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

#define MAX (8 << 10)

int B[MAX];
char E[MAX][MAX];
char buf[1 << 20];
int gN;

int dfs(int a)
{
	if (a == gN - 1)
		return 1;

	B[a] = 1;
	int i;
	FOR(i, 0, gN)
		if (B[i] == 0 && E[a][i] > 0 && dfs(i) != 0)
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
	gets(buf);
	sscanf(buf, "%d", &N);

	int i, j, k;
	VInt v[256];
	map<string, int> Map;
	FOR(i, 0, N)
	{
		gets(buf);
		stringstream ss(buf);
		string s;
		while (ss >> s)
		{
			if (Map.find(s) == Map.end())
				Map[s] = SIZE(Map);

			v[i].push_back(Map[s]);
		}
	}

	int m = SIZE(Map);
	gN = m + m + 2;
	CLEAR(E, 0);
	FOR(i, 2, N)
		FOR(j, 0, SIZE(v[i]))
		FOR(k, 0, j)
		{
			E[v[i][j]][m + v[i][k]] = 1;
			E[v[i][k]][m + v[i][j]] = 1;
		}

	FOR(i, 0, SIZE(v[0]))
		E[gN - 2][m + v[0][i]] = 1;
	FOR(i, 0, SIZE(v[1]))
		E[v[1][i]][gN - 1] = 1;

	FOR(i, 0, m)
		E[m + i][i] = 1;

	int flow = 0;
	while (true)
	{
		CLEAR(B, 0);
		if (dfs(gN - 2) == 0)
			break;

		++flow;
	}

	printf("Case #%d: %d\n", test + 1, flow);
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
