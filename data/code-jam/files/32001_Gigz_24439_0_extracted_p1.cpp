#include <algorithm>
#include <cstdio>
#include <string>
#include <vector>
#include <cmath>
#include <iostream>
#include <map>

using namespace std;

#define FOR(i,n) for(i = 0; i < (n); ++i)
#define SFOR(i,m,n) for(i = (m); i < (n); ++i)

#define lint __int64

int A[10001];
int B[10001];
int C[10001];
int n,q;
int FF[10001];
int U[10001];

int F(int v)
{
	int r1,r2;
	if (U[v] == 0)
	{
		U[v] = 1;
		if (v > (n - 1) / 2)
		{
			if (A[v] == q) FF[v] = 0;
		}
		else
		{
			r1 = F(v*2);
			r2 = F(v*2 + 1);
			if (q == B[v]) FF[v] = r1 + r2;
			else FF[v] = min(r1, r2);
			if (C[v] == 1)
			{
				if (q == B[v]) FF[v] = min(FF[v], min(r1,r2) + 1);
				else FF[v] = min(FF[v], r1 + r2 + 1);
			}
		}
	}
	return FF[v];
}

int main()
{
	FILE *fi, *fo;
	fi = fopen("p1.in", "rt");
	fo = fopen("p1.out", "wt");

	int t,tt;
	fscanf(fi, "%d", &tt);
	for(t = 0; t < tt; ++t)
	{
		int i,j,k;
		fscanf(fi, "%d%d", &n, &q);
		for(i = 1; i <= n; ++i)
		if (i <= (n-1) / 2)
			fscanf(fi,"%d%d",&B[i], &C[i]);
		else
			fscanf(fi, "%d", &A[i]);
		for (i = 0; i <= n; ++i) FF[i] = 10000;
		memset(U,0, sizeof(U));
		
		int res = F(1);
		if (res >= 10000)
		fprintf(fo, "Case #%d: IMPOSSIBLE\n", t+1);
		else
		fprintf(fo, "Case #%d: %d\n", t + 1, res);
	}

	fclose(fi);
	fclose(fo);
	return 0;
}