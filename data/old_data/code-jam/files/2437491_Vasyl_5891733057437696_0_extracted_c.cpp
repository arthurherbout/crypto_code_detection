#define _CRT_SECURE_NO_WARNINGS
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

#define INF 1e47
#define EPS 1e-7
#define MAX (1 << 18)

int X[64];
int Y[64];
double XX[64];
double YY[64];

void rotate(double x, double y, double& xx, double& yy, double a)
{
	double c = cos(a);
	double s = sin(a);

	xx = x*c + y*s;
	yy = -x*s + y*c;
}

double find(vector<double>& v, double& d)
{
	int n = SIZE(v);
	int a = n/2;
	int b = a - 1;
	d = fabs(v[a] - v[b]);

	return (v[a] + v[b])/2;
}

int SolveTest(int test)
{
	int N;
	scanf("%d", &N);
	int M = 4*N;

	int i, j;
	FOR(i, 0, M)
		scanf("%d%d", &X[i], &Y[i]);

	double pi = 2*acos(0.0);
	double resX = INF;
	double resY = INF;
	double resXX = INF;
	double resYY = INF;
	double d = 0;
	FOR(i, 1, MAX)
	{
		double a = pi/i;
		FOR(j, 0, M)
			rotate(X[j], Y[j], XX[j], YY[j], a);

		vector<double> x(XX, XX + M);
		vector<double> y(YY, YY + M);

		sort(ALL(x));
		sort(ALL(y));

		double da = 0;
		double db = 0;
		double xx = find(x, da);
		double yy = find(y, db);

		if(d > min(da, db))
			continue;

		int cnt[] = {0, 0, 0, 0};
		FOR(j, 0, M)
		{
			int pos = 0;
			if(XX[j] > xx)
				pos |= 1;
			if(YY[j] > yy)
				pos |= 2;

			++cnt[pos];
		}

		FOR(j, 0, 4)
			if(cnt[j] != N)
				break;

		if(j == 4)
		{
			d = min(da, db);
			rotate(xx, yy, resX, resY, -a);
			rotate(xx + 1, yy, resXX, resYY, -a);
			break;
		}
	}

	if(resX == INF)
		printf("Case #%d: IMPOSSIBLE\n", test + 1);
	else
		printf("Case #%d: %.7lf %.7lf %.7lf %.7lf\n", test + 1, resX, resY, resXX, resYY);

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
