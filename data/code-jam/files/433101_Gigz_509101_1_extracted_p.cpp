#include <cstdio>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <map>
#include <set>
#include <utility>

#define FOR(i,n) for(i=0; i<(n); ++i)

using namespace std;

int G[1000];
int A[1000][1000];
int P[1000];

int r,k,n;

int doit(int p)
{
	int i,j;
	int cur = 0;
	FOR(i, n)
	{
		if (cur + A[p][i] > k) break;
		cur += A[p][i];
	}
	P[p] = cur;

	if (i == n)
	{
		return 0;
	}

	for(j = i; j < n; ++j) A[p+1][j-i] = A[p][j];
	FOR(j, i) A[p+1][n-i+j] = A[p][j];

	FOR(i, p + 1)
	{
		int fl = 1;
		FOR(j, n) if (A[i][j] != A[p+1][j]) {fl = 0; break;}
		if (fl) return i;
	}

	return -1;
}

int main()
{
	int t, tt;
	FILE *fi = fopen("a.in", "rt");
	FILE *fo = fopen("a.out", "wt");
	fscanf(fi, "%d", &tt);
	for (t = 0; t < tt; ++t)
	{
		int i,j;
		fscanf(fi, "%d%d%d", &r, &k, &n);
		for (i = 0; i < n; ++i) 
		{
			fscanf(fi, "%d", &G[i]);
			A[0][i] = G[i];
		}
		i = 0;
		while ((j = doit(i)) == -1)
		{
			++i;
		}

		__int64 res = 0;

		if (r - 1 <= i)
		{
			FOR(j, r) res += P[j];
			fprintf(fo, "Case #%d: %I64d\n", t+1, res);
			continue;
		}

		int m;
		FOR(m, j) res += P[m];
		r -= j;
		FOR(m, r % (i - j + 1)) res += P[m + j];
		__int64 cyc = 0;
		for(m = j; m <= i; ++m) cyc += P[m];
		res += cyc * (r / (i - j + 1));
		fprintf(fo, "Case #%d: %I64d\n", t+1, res);
	}
	fclose(fi);
	fclose(fo);
	return 0;
}