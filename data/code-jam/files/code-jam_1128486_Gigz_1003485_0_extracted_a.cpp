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

int D[1000];
int n;
__int64 d;

double simulate(int p1, int p2)
{
	double ret = 0;
	int i;
	if (p1 == 3 && p2 == 5)
	{
		int q = 1;
	}

	FOR(i, n)
	{
		bool boosted = (i == p1 || i == p2);
		if (!boosted)
		{
			ret += D[i] * 2;
		}
		else
		{
			if (ret >= d) ret += D[i];
			else if (ret + D[i] * 2 <= d) ret += D[i] * 2;
			else
			{
				ret = d + (D[i] - (d - ret) / 2.0);
			}
		}
	}
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
		int C[1000];
		fscanf(fi, "%d%I64d%d%d", &l, &d, &n, &c);
		FOR(i, c) fscanf(fi, "%d", &C[i]);
		FOR(i, n) D[i] = C[i%c];

		int p1, p2;
		
		double res = 1000000000;
		res *= res;
		SFOR(p1, (l>0) ? 0 : n, n+1) SFOR(p2, (l > 1) ? p1+1 : n, n+1)
		{
			double cur = simulate(p1, p2);
			if (cur < res) res = cur;
		}

		fprintf(fo, "Case #%d: %I64d\n", t+1, (__int64)res);
	}

	fclose(fo);
	fclose(fi);
	return 0;
}