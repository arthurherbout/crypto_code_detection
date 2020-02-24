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

int Left[1 << 10][32];
int Right[1 << 10][32];
int B[1 << 10];
int C[1 << 10];
int From[1 << 10];
int To[1 << 10];
VInt E[1 << 10];
int Res[1 << 10][1 << 10];

int dfs(int a, int b)
{
	B[a] = b;

	int i;
	FOR(i, 0, C[a])
	{
		int c = E[a][i];
		if(B[c] == B[a])
			return 1;

		if(B[c] == -1 && dfs(c, b ^ 1))
			return 1;
	}

	return 0;
}

int F(int);
int G(int a, int b)
{
	int& res = Res[a][b];
	if(res != -1)
		return res;

	res = 0;
	int i, j;
	FOR(i, 0, C[a])
		FOR(j, 0, C[b])
			if(E[a][i] == E[b][j])
			{
				int c = E[a][i];
				if(B[c] == B[a] || B[c] == B[b])
					return res = 1;

				if(B[c] == -1)
				{
					B[c] = 0;
					while(B[c] == B[a] || B[c] == B[b])
						++B[c];

					if(F(c))
						return res = 1;
				}
			}

	return res;
}

int F(int a)
{
	int i;
	FOR(i, 0, C[a])
	{
		int b = E[a][i];
		if(B[b] == B[a])
			return 1;

		if(B[b] != -1 && G(a, b))
			return 1;
	}

	FOR(i, 0, C[a])
	{
		int b = E[a][i];
		if(B[b] == -1)
		{
			B[b] = (B[a] + 1) % 3;
			if(F(b))
				return 1;
		}
	}

	return 0;
}

int SolveTest(int test)
{
	int N;
	scanf("%d", &N);
	int i, j;
	FOR(i, 0, N)
		E[i].clear();

	vector<PII> v;
	FOR(i, 0, N)
	{
		int x, y;
		scanf("%d%d", &y, &x);
		v.PB(PII(x, y));
	}

	sort(ALL(v));

	FOR(i, 0, 32)
	{
		From[i] = 0;
		while(From[i] != N && v[ From[i] ].first < i)
			++From[i];

		To[i] = From[i];
		while(To[i] != N && v[ To[i] ].first <= i)
			++To[i];
	}

	CLEAR(Left, -1);
	CLEAR(Right, -1);
	int cnt = 0;
	FOR(i, 0, N)
	{
		FOR(j, v[i].first - 1, v[i].first + 2)
		{
			int pos = From[j];
			while(pos != To[j] && v[pos].second < v[i].second)
				++pos;

			if(j == v[i].first)
				++pos;
			if(From[j] <= pos && pos < To[j])
			{
				Right[i][j] = pos;
				E[i].PB(pos);
				E[pos].PB(i);
				++cnt;
			}

			if(j == v[i].first)
				--pos;
			--pos;

			if(From[j] <= pos && pos < To[j])
			{
				Left[i][j] = pos;
				E[i].PB(pos);
				E[pos].PB(i);
				++cnt;
			}
		}
	}

	FOR(i, 0, N)
		C[i] = SIZE(E[i]);

	if(cnt == 0)
	{
		printf("Case #%d: 1\n", test + 1);
		return 0;
	}

	CLEAR(B, -1);
	FOR(i, 0, N)
		if(B[i] == -1 && dfs(i, 0))
			break;

	if(i == N)
	{
		printf("Case #%d: 2\n", test + 1);
		return 0;
	}

	CLEAR(B, -1);
	CLEAR(Res, -1);
	FOR(i, 0, N)
		if(B[i] == -1)
		{
			B[i] = 0;
			if(F(i))
				break;
		}

	if(i == N)
	{
		printf("Case #%d: 3\n", test + 1);
		return 0;
	}

	printf("Case #%d: 4\n", test + 1);
	return 0;
}

int main()
{
//	freopen("c.in", "r", stdin);
//	freopen("c.out", "w", stdout);

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
