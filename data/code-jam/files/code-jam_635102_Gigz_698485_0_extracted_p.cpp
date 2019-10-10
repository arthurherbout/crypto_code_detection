#include <cstdio>
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <map>
#include <set>
#include <utility>
using namespace std;

#define FOR(i,n) for(i=0; i<(n); ++i)
#define SFOR(i,m,n) for(i=(m); i<(n); ++i)

int A[20][2000];

int main()
{
	int t, tt;
	FILE *fi = fopen("a.in", "rt");
	FILE *fo = fopen("a.out", "wt");
	fscanf(fi, "%d", &tt);
	for (t = 0; t < tt; ++t)
	{
		int n, m;
		fscanf(fi, "%d", &n);
		m = 1<<n;
		int i,j;

		memset(A, 100, sizeof(A));

		int res = 0;

		FOR(i, m) {fscanf(fi, "%d", &A[0][i]); }

		int v;
		FOR(i, n) FOR(j, 1<<i) fscanf(fi, "%d", &v);

		FOR(i, n)
		{
			for(j=0; j<(1<<(n-i)); j += 2){
				if (A[i][j] == 0 || A[i][j+1] == 0) ++res;
				if (A[i+1][j/2] > A[i][j] - 1) A[i+1][j/2] = max(0, A[i][j] - 1);
				if (A[i+1][j/2] > A[i][j+1] - 1) A[i+1][j/2] = max(0, A[i][j+1] - 1);
			}
		}

		fprintf(fo, "Case #%d: %d\n", t+1, res);
	}
	fclose(fi);
	fclose(fo);
	return 0;
}