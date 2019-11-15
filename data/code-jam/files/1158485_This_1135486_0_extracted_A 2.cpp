/*
 * 2011-06-11  Martin  <Martin@Martin-desktop>

 * 
 */
#include <iostream>
#include <fstream>
#include <algorithm>
#include <set>
#include <vector>

using namespace std;

#define rep(i, n) for (int i = 0; i < n; ++ i)
#define pb push_back
#define MaxiN 105

int W, L, U, G;
int XL[MaxiN], YL[MaxiN], XU[MaxiN], YU[MaxiN];
double Area[MaxiN * 2];

inline double getY(double y1, double y2, double a, double b)
{
	return (y1 * b + y2 * a) / (a + b);
}

inline double f(double x)
{
	double y = 0;
	rep (i, L)
		if (XL[i] <= x && x <= XL[i + 1])
		{
			y -= getY(YL[i], YL[i + 1], x - (double) XL[i], (double) XL[i + 1] - x);
			break;
		}
	rep (i, U)
		if (XU[i] <= x && x <= XU[i + 1])
		{
			y += getY(YU[i], YU[i + 1], x - (double) XU[i], (double) XU[i + 1] - x);
			break;
		}
	return y;
}

inline void Solve()
{
	scanf("%d%d%d%d", &W, &L, &U, &G);
	rep (i, L)
		scanf("%d%d", &XL[i], &YL[i]);
	rep (i, U)
		scanf("%d%d", &XU[i], &YU[i]);
	set <int> lsh;
	lsh.clear();
	lsh.insert(0);
	lsh.insert(W);
	rep (i, L)
		lsh.insert(XL[i]);
	rep (i, U)
		lsh.insert(XU[i]);
	vector <int> lshh;
	lshh.clear();
	for (set <int> :: iterator it = lsh.begin(); it != lsh.end(); ++ it)
		lshh.pb(*it);
	double area = 0;
	Area[0] = 0;
	for (int i = 1; i < (int) lshh.size(); ++ i)
	{
		Area[i] = (f(lshh[i - 1]) + f(lshh[i])) * (double) (lshh[i] - lshh[i - 1]) / 2.0;
		area += Area[i];
	}
	double average = area / (double) G;
	double sum = 0;
	for (int k = 1, i = 0; k < G; ++ k)
	{
		while (sum < average * k)
		{
			++ i;
			sum += Area[i];
		}
		double s = lshh[i - 1], t = lshh[i];
		double ft = f(t);
		double delta = sum - average * k;
		double l = s, r = t;
		while (r - l > 1e-8)
		{
			double mid = (l + r) / 2.0;
			if ((f(mid) + ft) * (t - mid) / 2.0 > delta)
				l = mid;
			else
				r = mid;
		}
		printf("%.9lf\n", r);
	}
}

int main()
{
	int TestCase;
	scanf("%d", &TestCase);
	for (int i = 1; i <= TestCase; ++ i)
	{
		printf("Case #%d:\n", i);
		Solve();
	}
	return 0;
}
