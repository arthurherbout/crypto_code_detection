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

int X[1 << 10];
int Y[1 << 10];
int XX[1 << 10];
int YY[1 << 10];

double F(double x, double y, double xx, double yy, double lim)
{
	if(lim <= x)
		return 0;
	if(lim >= xx)
		return (xx - x)*(y + yy)/2;

	double xxx = lim;
	double yyy = (y*(xx - xxx) + yy*(xxx - x))/(xx - x);
	return (xxx - x)*(yyy + y)/2;
}

int SolveTest(int test)
{
	int W, L, U, G;
	scanf("%d%d%d%d", &W, &L, &U, &G);

	int i, j, k;
	FOR(i, 0, L)
		scanf("%d%d", &X[i], &Y[i]);

	FOR(i, 0, U)
		scanf("%d%d", &XX[i], &YY[i]);

	double area = 0;
	FOR(i, 0, L - 1)
		area -= F(X[i], Y[i], X[i + 1], Y[i + 1], W);
	FOR(i, 0, U - 1)
		area += F(XX[i], YY[i], XX[i + 1], YY[i + 1], W);

	printf("Case #%d:\n", test + 1);
	FOR(i, 1, G)
	{
		double t = area*i/G;
		double Min = 0, Max = W;
		FOR(j, 0, 64)
		{
			double Mid = (Max + Min)/2;
			double r = 0;
			FOR(k, 0, L - 1)
				r -= F(X[k], Y[k], X[k + 1], Y[k + 1], Mid);
			FOR(k, 0, U - 1)
				r += F(XX[k], YY[k], XX[k + 1], YY[k + 1], Mid);

			if(r < t)
				Min = Mid;
			else
				Max = Mid;
		}

		printf("%.7lf\n", Max);
	}

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
