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

VInt E[1 << 11];
int C[1 << 11];
int D0[1 << 11];
int D1[1 << 11];
int L[1 << 11];
int R[1 << 11][1 << 11];
int A[1 << 11][1 << 11];
int gP;

int F(int a, int b)
{
	int& res = R[a][b];
	if(res != -1)
		return res;

	res = 0;

	int i, j;
	if(L[a] == 0)
	{
		FOR(i, 0, gP)
			if(A[a][i] != 0 || A[b][i] != 0)
				++res;
		
		return res;
	}

	FOR(i, 0, C[a])
	{
		int z = E[a][i];
		if(L[z] == L[a] - 1)
		{
			int r = 0;
			FOR(j, 0, C[b])
			{
				int y = E[b][j];
				if(A[a][y] == 0 && A[z][y] == 0)
					++r;
			}

			res = max(res, F(z, a) + r);
		}
	}

	return res;
}

int SolveTest(int test)
{
	int p, w;
	scanf("%d%d", &p, &w);

	int i, j;
	FOR(i, 0, w)
	{
		int a, b;
		char buf[64];
		scanf("%s", buf);
		int len = strlen(buf);
		replace(buf, buf + len, ',', ' ');
		sscanf(buf, "%d%d", &a, &b);
		E[a].PB(b);
		E[b].PB(a);
	}

	E[p].PB(0);
	E[0].PB(p);
	++p;

	FOR(i, 0, p)
		C[i] = SIZE(E[i]);

	CLEAR(A, 0);
	FOR(i, 0, p)
		FOR(j, 0, C[i])
			A[i][ E[i][j] ] = 1;

	FOR(i, 0, p)
		A[i][i] = 1;

	queue<int> Q;
	CLEAR(D0, -1);
	D0[0] = 0;
	Q.push(0);
	while(!Q.empty())
	{
		int a = Q.front();
		Q.pop();

		FOR(i, 0, C[a])
		{
			int b = E[a][i];
			if(D0[b] == -1)
			{
				D0[b] = D0[a] + 1;
				Q.push(b);
			}
		}
	}

	CLEAR(D1, -1);
	D1[1] = 0;
	Q.push(1);
	while(!Q.empty())
	{
		int a = Q.front();
		Q.pop();

		FOR(i, 0, C[a])
		{
			int b = E[a][i];
			if(D1[b] == -1)
			{
				D1[b] = D1[a] + 1;
				Q.push(b);
			}
		}
	}

	int dist = D0[1];
	CLEAR(L, -1);
	FOR(i, 0, p)
		if(D0[i] != -1 && D1[i] != -1 && D0[i] + D1[i] == dist)
			L[i] = D0[i] + 1;

	L[p - 1] = 0;

	CLEAR(R, -1);
	int res = 0;
	gP = p;
	FOR(i, 0, p)
		if(L[i] == dist)
			FOR(j, 0, C[i])
				if(L[ E[i][j] ] == dist - 1)
				res = max(res, F(E[i][j], i));

	printf("Case #%d: %d %d\n", test + 1, dist - 1, res - dist - 1);

	FOR(i, 0, p)
		E[i].clear();

	return 0;
}

int main()
{
	freopen("d.in", "r", stdin);
	freopen("d.out", "w", stdout);

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
