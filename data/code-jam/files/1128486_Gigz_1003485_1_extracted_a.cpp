#include <cstdio>
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <deque>
#include <queue>
#include <utility>
#include <cmath>
using namespace std;

#define FOR(i,n) for(i=0; i<(n); ++i)
#define SFOR(i,m,n) for(i=(m); i<(n); ++i)

int D[1000100];
int T[1000100];
int C[1000100];
int n;
__int64 d;

double calc(int p, int l)
{
	double ret = 0;
	int i;
	SFOR(i, p, n+1) T[i - p] = D[i];
	sort(T, T+n-p);
	i = 0;
	FOR(i, min(n-p, l)) ret += T[n-p-i-1]; 
	while (i < n-p) {ret += T[n-p-i-1] * 2; ++i;}
	return ret;
}

int main()
{
	FILE *fi = fopen("a.txt", "rt");
	FILE *fo = fopen("a.out", "wt");

	int t,tt;

	fscanf(fi, "%d", &tt);
	FOR(t,tt)
	{
		int i,l,c;
		fscanf(fi, "%d%I64d%d%d", &l, &d, &n, &c);
		FOR(i, c) fscanf(fi, "%d", &C[i]);
		FOR(i, n) D[i] = C[i%c];

		bool fl = false;
		double res = 0;
		FOR(i, n)
		{
			if (res + 2*D[i] > d && l != 0)
			{
				fl = true;
				break;
			}
			res += 2*D[i];
		}

		if (fl)
		{
			res = min(d + (D[i] - (d - res) / 2.0) + calc(i+1, l - 1), res + 2*D[i] + calc(i+1, l));
		}

		fprintf(fo, "Case #%d: %I64d\n", t+1, (__int64)res);
	}

	fclose(fo);
	fclose(fi);
	return 0;
}