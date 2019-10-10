#include <cstdio>
#include <iostream>
#include <set>
#include <map>
#include <sstream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <utility>

using namespace std;

#define FOR(i,n) for((i) = 0; (i) < (n); ++(i))
#define SFOR(i,m,n) for((i) = (m); (i) < (n); ++(i))

int X[3], Y[3], R[3];

double doit(int a, int b, int c)
{
	double d = (sqrt(1.0*(X[a] - X[b])*(X[a] - X[b]) + 1.0*(Y[a] - Y[b])*(Y[a] - Y[b])) + R[a] + R[b]) / 2.0;
	return max(d, R[c]);
}

int wmain(void)
{
	FILE *fi, *fo;	
	fi = fopen("a.in", "rt");
	fo = fopen("a.out", "wt");
	
	int t, tt;
	fscanf(fi, "%d", &tt);
	FOR(t, tt)
	{
		int n,m,i,j;
		fscanf(fi, "%d", &n);
		FOR(i,n) fscanf(fi, "%d%d%d", &X[i], &Y[i], &R[i]);
		if (n == 1) {fprintf(fo, "Case #%d: %d\n", t+1, R[0]); continue;}
		if (n == 2) {fprintf(fo, "Case #%d: %d\n", t+1, max(R[0], R[1])); continue;}
		double res, cur;
		res = doit(0, 1, 2);
		cur = doit(0, 2, 1); if (cur < res) res = cur;
		cur = doit(1, 2, 0); if (cur < res) res = cur;
		fprintf(fo, "Case #%d: %lf\n", t+1, res);
	}

	fclose(fi);
	fclose(fo);

	return 0;
}