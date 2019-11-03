#include <iostream>
#include <string>
#include <string.h>
#include <cstring>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <bitset>
#include <queue>
#pragma comment (linker, "/STACK:256000000")

using namespace std;

int w, n, m, g;
int x1[110], y1[110], x2[110], y2[110];
vector < double > res;

double get_area(double key)
{
	double area = 0.0;
	for (int i = 2; i <= m; i ++)
		if (x2[i] < key) area += (double)(y2[i - 1] + y2[i]) / 2.0 * (double)(x2[i] - x2[i - 1]); else
		{
			double p = (double)(y2[i] - y2[i - 1]) * (key - (double)(x2[i - 1])) / (double)(x2[i] - x2[i - 1]);
			area += (double)(key - x2[i - 1]) / 2.0 * ((double)(y2[i - 1]) + (double)(y2[i - 1]) + p);
			break;
		}
	for (int i = 2; i <= n; i ++)
		if (x1[i] < key) area -= (double)(y1[i - 1] + y1[i]) / 2.0 * (double)(x1[i] - x1[i - 1]); else
		{
			double p = (double)(y1[i] - y1[i - 1]) * (key - (double)(x1[i - 1])) / (double)(x1[i] - x1[i - 1]);
			area -= (double)(key - x1[i - 1]) / 2.0 * ((double)(y1[i - 1]) + (double)(y1[i - 1]) + p);
			break;
		}
	return area;
}

void solve(int test)
{
	scanf("%d%d%d%d", &w, &n, &m, &g);
	for (int i = 1; i <= n; i ++)
		scanf("%d%d", &x1[i], &y1[i]);
	for (int i = 1; i <= m; i ++)
		scanf("%d%d", &x2[i], &y2[i]);

	double area = 0.0;
	for (int i = 2; i <= m; i ++)
		area += (double)(y2[i - 1] + y2[i]) / 2.0 * (double)(x2[i] - x2[i - 1]);
	for (int i = 2; i <= n; i ++)
		area -= (double)(y1[i - 1] + y1[i]) / 2.0 * (double)(x1[i] - x1[i - 1]);

	double piece = area / (double)(g);
	double l = 0.0, r = (double)(w);
	res.clear();
	for (int i = 1; i < g; i ++)
	{
		double cl = 0, cr = w;
		int t = 0;
		while (t < 100)
		{
			double key = (cl + cr) / 2.0;
			if (get_area(key) > piece * (double)(i)) cr = key; else cl = key;
			t ++;
		}
		res.push_back(cl);
	}
	printf("Case #%d:\n", test);
	for (int i = 0; i < res.size(); i ++)
		printf("%.10lf\n", res[i]);
}

int main()
{
	freopen("input.txt","r",stdin);
	freopen("output.txt","w",stdout);
	int test;
	scanf("%d\n", &test);
	for (int i = 1; i <= test; i ++)
	{
		cerr << i << endl;
		solve(i);
	}
	return 0;
}