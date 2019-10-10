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

int X0[1 << 10];
int Y0[1 << 10];
int X1[1 << 10];
int Y1[1 << 10];
int R[1 << 10];

int f(int a, int b, int c, int d)
{
	if (d < a)
		return a - d;
	if (b < c)
		return c - b;

	return 0;
}

int f(int a, int b)
{
	return max(f(X0[a], X1[a], X0[b], X1[b]), f(Y0[a], Y1[a], Y0[b], Y1[b]));
}

int SolveTest(int test)
{
	int W, H, B;
	scanf("%d%d%d", &W, &H, &B);

	int i, j;
	FOR(i, 0, B)
	{
		int x0, y0, x1, y1;
		scanf("%d%d%d%d", &x0, &y0, &x1, &y1);

		X0[i] = x0;
		Y0[i] = y0;
		X1[i] = x1 + 1;
		Y1[i] = y1 + 1;
	}

	priority_queue<PII> Q;
	FOR(i, 0, B)
	{
		R[i] = X0[i];
		Q.push(PII(-R[i], i));
	}

	while (!Q.empty())
	{
		i = Q.top().second;
		int value = -Q.top().first;
		Q.pop();
		if (value != R[i])
			continue;

		FOR(j, 0, B)
		if (j != i)
		{
			int v = value + f(i, j);
			if (R[j] > v)
			{
				R[j] = v;
				Q.push(PII(-R[j], j));
			}
		}
	}

	int res = W;
	FOR(i, 0, B)
		res = min(res, R[i] + W - X1[i]);

	printf("Case #%d: %d\n", test + 1, res);
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
