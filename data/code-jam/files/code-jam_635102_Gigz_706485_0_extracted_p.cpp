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

int A[2][101][101];

int main()
{
	int t, tt;
	FILE *fi = fopen("a.in", "rt");
	FILE *fo = fopen("a.out", "wt");
	fscanf(fi, "%d", &tt);
	for (t = 0; t < tt; ++t)
	{
		int n, res = 0;;
		fscanf(fi, "%d", &n);
		int i,j,k;
		memset(A, 0, sizeof(A));
		FOR(k,n)
		{
			int x1, y1, x2, y2;
			fscanf(fi, "%d%d%d%d", &x1, &y1, &x2, &y2);
			SFOR(i,x1,x2+1) SFOR(j,y1,y2+1) A[0][i][j] = 1;
		}

		int q = 0;
		FOR(i, 101) FOR(j, 101) q+=A[0][i][j];

		while (q>0)
		{
			q = 0;
			++res;
			SFOR(i, 1, 101) SFOR(j, 1, 101)
			{
				if (!A[(res-1)%2][i][j]) A[res%2][i][j] = A[(res-1)%2][i-1][j] & A[(res-1)%2][i][j-1];
				else A[res%2][i][j] = A[(res-1)%2][i-1][j] | A[(res-1)%2][i][j-1];
				q += A[res%2][i][j];
			}
		}
		
		fprintf(fo, "Case #%d: %d\n", t+1, res);
	}
	fclose(fi);
	fclose(fo);
	return 0;
}