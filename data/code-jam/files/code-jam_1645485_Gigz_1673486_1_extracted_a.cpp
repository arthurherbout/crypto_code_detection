#include <iostream>
#include <cstring>
#include <cstdio>
#include <string>
#include <algorithm>
#include <hash_map>
#include <hash_set>

using namespace std;

#define FOR(i,n) for((i) = 0; (i) < (n); ++(i))

FILE *fi, *fo;

double P[100050];
double R[100050];

void mainImpl(int _c)
{
	int a, b, i, j;
	fscanf(fi, "%d%d", &a, &b);
	FOR(i, a) fscanf(fi, "%lf", &P[i]);

	R[0] = 1.0;
	FOR(i, a)
	{
		R[i + 1] = R[i] * P[i];
	}

	double res = b + 2.0;
	FOR(i, a + 1)
	{
		double cur = R[a-i] * (i + b - a + i + 1.0) + (1 - R[a-i]) * (i + b - a + i + 1.0 + b + 1.0);
		res = min(res, cur);
	}

	fprintf(fo, "Case #%d: %.9lf\n", _c, res);
}


int main()
{
	fi = fopen("a.in", "rt");
	fo = fopen("a.out", "wt");

	int t, tt;
	fscanf(fi, "%d", &tt);

	FOR(t, tt) mainImpl(t + 1);

	fclose(fo);
	fclose(fi);
	return 0;
}