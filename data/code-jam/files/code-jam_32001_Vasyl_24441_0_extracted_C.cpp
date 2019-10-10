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

#define MAX 64

int X[1 << 10];
int Y[1 << 10];
int Z[1 << 10];
int P[1 << 10];

void F(double x, double y, double z, double& xx, double& yy, double& zz)
{
	xx = x + y + z;
	yy = -x + y + z;
	zz = -x - y + z;
}

int main()
{
	freopen("C.in", "r", stdin);
	freopen("C.out", "w", stdout);

	int T, t;
	scanf("%d", &T);
	FOR(t, 0, T)
	{
		int N;
		scanf("%d", &N);

		int i, j, k;
		FOR(i, 0, N)
			scanf("%d%d%d%d", &X[i], &Y[i], &Z[i], &P[i]);

		double Min = 0.0, Max = 1e7;
		FOR(i, 0, MAX)
		{
			double Mid = (Max + Min)/2;

			double xxMin = -1e7;
			double xxMax = 1e7;
			double yyMin = -1e7;
			double yyMax = 1e7;
			double zzMin = -1e7;
			double zzMax = 1e7;
			FOR(j, 0, N)
			{
				double temp = Mid*P[j];
				double x[6], y[6], z[6];
				F(X[j] + temp, Y[j] + 0.0, Z[j] + 0.0, x[0], y[0], z[0]);
				F(X[j] - temp, Y[j], Z[j] + 0.0, x[1], y[1], z[1]);
				F(X[j] + 0.0, Y[j] + temp, Z[j] + 0.0, x[2], y[2], z[2]);
				F(X[j] + 0.0, Y[j] - temp, Z[j] + 0.0, x[3], y[3], z[3]);
				F(X[j] + 0.0, Y[j] + 0.0, Z[j] + temp, x[4], y[4], z[4]);
				F(X[j] + 0.0, Y[j] + 0.0, Z[j] - temp, x[5], y[5], z[5]);

				double xMin = x[0];
				double xMax = x[0];
				double yMin = y[0];
				double yMax = y[0];
				double zMin = z[0];
				double zMax = z[0];
				
				FOR(k, 0, 6)
				{
					xMin = min(xMin, x[k]);
					xMax = max(xMax, x[k]);
					yMin = min(yMin, y[k]);
					yMax = max(yMax, y[k]);
					zMin = min(zMin, z[k]);
					zMax = max(zMax, z[k]);
				}

				xxMin = max(xxMin, xMin);
				xxMax = min(xxMax, xMax);
				yyMin = max(yyMin, yMin);
				yyMax = min(yyMax, yMax);
				zzMin = max(zzMin, zMin);
				zzMax = min(zzMax, zMax);
			}

			if(xxMin <= xxMax && yyMin <= yyMax && zzMin <= zzMax)
				Max = Mid;
			else
				Min = Mid;
		}

		printf("Case #%d: %.7lf\n", t + 1, Max);
	}

	return 0;
};
