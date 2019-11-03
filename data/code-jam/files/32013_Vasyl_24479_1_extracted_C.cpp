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

double dist(double x, double y)
{
	return sqrt(x*x + y*y);
}

double F(double x, double y, double xx, double yy, double r)
{
	if(dist(x, y) >= r)
		return 0.0;
	if(dist(xx, yy) <= r)
		return (xx - x)*(yy - y);

	vector< pair<double, double> > v, h, c;
	v.PB(MP(x, y));
	v.PB(MP(xx, y));
	v.PB(MP(xx, yy));
	v.PB(MP(x, yy));

	int i;
	FOR(i, 0, SIZE(v))
	{
		if(dist(v[i].first, v[i].second) <= r)
			h.PB(v[i]);

		x = v[i].first;
		y = v[i].second;
		xx = v[(i + 1) % SIZE(v)].first;
		yy = v[(i + 1) % SIZE(v)].second;
		double dx = xx - x;
		double dy = yy - y;

		double A = dx*dx + dy*dy;
		double B = x*dx + y*dy;
		double C = x*x + y*y - r*r;
		double D = B*B - A*C;

		if(D >= 0)
		{
			double k1 = (-B - sqrt(D))/A;
			double k2 = (-B + sqrt(D))/A;

			if(k1 >= 0 && k1 <= 1)
			{
				pair<double, double> temp(x + k1*dx, y + k1*dy);
				h.PB(temp);
				if(c.empty() || c[ SIZE(c) - 1 ] != temp)
					c.PB(temp);
			}
			else if(k2 >= 0 && k2 <= 1)
			{
				pair<double, double> temp(x + k2*dx, y + k2*dy);
				h.PB(temp);
				if(c.empty() || c[ SIZE(c) - 1 ] != temp)
					c.PB(temp);
			}
		}
	}

	double res = 0;
	int N = SIZE(h);
	FOR(i, 0, N)
	{
		x = h[i].first;
		y = h[i].second;
		xx = h[(i + 1) % N].first;
		yy = h[(i + 1) % N].second;

		res += x*yy - xx*y;
	}

	res = fabs(res)/2;

	if(SIZE(c) == 2)
	{
		x = c[0].first;
		y = c[0].second;
		xx = c[1].first;
		yy = c[1].second;

		res += fabs(atan2(yy, xx) - atan2(y, x))*r*r/2;
		res -= fabs(x*yy - xx*y)/2;
	}

	return res;
}

int main()
{
	freopen("C.in", "r", stdin);
	freopen("C.out", "w", stdout);
	
	int T, test;
	scanf("%d", &T);
	FOR(test, 0, T)
	{
		double f, R, t, r, g;
		scanf("%lf%lf%lf%lf%lf", &f, &R, &t, &r, &g);
		double square = acos(0.0)*R*R/2;
		double step = r + r + g;
		double limit = R - t - f;
		double res = 0;

		int i, j;
		if(g - f - f > 0 && limit > 0)
			for(i = 0; i*step < limit; ++i)
				for(j = 0; j*step < limit; ++j)
					res += F(i*step + r + f, j*step + r + f, i*step + r + g - f, j*step + r + g - f, limit);

		printf("Case #%d: %.9lf\n", test + 1, 1 - res/square);
	}
	
	return 0;
};
