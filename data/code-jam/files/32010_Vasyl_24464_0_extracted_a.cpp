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

#define EPS 1e-5

int X[4];
int Y[4];
int X1[4];
int Y1[4];
int X2[4];
int Y2[4];

double dist(double x, double y){ return sqrt(x*x + y*y); }

int F(double x, double y, int x1, int y1, int x2, int y2, int x3, int y3)
{
	double dx = x - x1;
	double dy = y - y1;
	double dx1 = x2 - x1;
	double dy1 = y2 - y1;
	double dx2 = x3 - x1;
	double dy2 = y3 - y1;

	double cross1 = dx*dy1 - dy*dx1;
	double cross2 = dx*dy2 - dy*dx2;

	if(cross1*cross2 > 0)
		return 0;

	return 1;
}

int F()
{
	int i;
	double a[3];
	double k[3];
	FOR(i, 0, 3)
	{
		double dx1 = X1[(i + 1) % 3] - X1[i];
		double dy1 = Y1[(i + 1) % 3] - Y1[i];
		double dx2 = X2[(i + 1) % 3] - X2[i];
		double dy2 = Y2[(i + 1) % 3] - Y2[i];
		
		a[i] = atan2(dy2, dx2) - atan2(dy1, dx1);
		k[i] = dist(dx1, dy1)/dist(dx2, dy2);

		a[i] = atan2(sin(a[i]), cos(a[i]));
	}

	FOR(i, 0, 3)
		if(fabs(a[i] - a[0]) > EPS || fabs(k[i] - k[0]) > EPS)
			return 0;

	double c = cos(a[0]);
	double s = sin(a[0]);
	double K = k[0];

	double c1 = K*(X2[0]*c + Y2[0]*s) - X1[0];
	double c2 = K*(-X2[0]*s + Y2[0]*c) - Y1[0];

	double a1 = K*c - 1;
	double a2 = -K*s;
	double b1 = K*s;
	double b2 = K*c - 1;

	double d = a1*b2 - a2*b1;
	double d1 = c1*b2 - c2*b1;
	double d2 = a1*c2 - a2*c1;

	double x = d1/d;
	double y = d2/d;

	FOR(i, 0, 3)
		if(F(x, y, X2[i], Y2[i], X2[(i + 1) % 3], Y2[(i + 1) % 3], X2[(i + 2) % 3], Y2[(i + 2) % 3]) == 0)
			break;

	if(i < 3)
		return 0;

	printf("%.5lf %.5lf\n", x, y);
	return 1;
}

int SolveTest(int test)
{
	scanf("%d%d%d%d%d%d", &X1[0], &Y1[0], &X1[1], &Y1[1], &X1[2], &Y1[2]);
	scanf("%d%d%d%d%d%d", &X[0], &Y[0], &X[1], &Y[1], &X[2], &Y[2]);

	printf("Case #%d: ", test + 1);

	int found = 0;
	int I[] = {0, 1, 2};
	do
	{
		int i;
		FOR(i, 0, 3)
		{
			X2[i] = X[ I[i] ];
			Y2[i] = Y[ I[i] ];
		}

		if(F())
		{
			found = 1;
			break;
		}
	}
	while(next_permutation(I, I + 3));

	if(found == 0)
		printf("No Solution\n");

	return 0;
}

int main()
{
	freopen("a.in", "r", stdin);
	freopen("a.out", "w", stdout);

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
