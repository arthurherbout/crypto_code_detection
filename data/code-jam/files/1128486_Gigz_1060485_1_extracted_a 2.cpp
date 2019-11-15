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

char readchar(FILE* f)
{
	char ret = 0;
	while (ret != '.' && ret != '#') fscanf(f, "%c", &ret);
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
		int A[100][100];
		int i,j,m,n;
		bool fl = true;

		fscanf(fi, "%d%d", &n, &m);
		FOR(i,n) FOR(j,m) A[i][j] = readchar(fi);
		FOR(i,n) FOR(j,m) if (A[i][j] == '#')
		{
			if (i == n-1 || j == m-1) {fl=false; goto imp;}
			if (A[i+1][j] != '#' || A[i][j+1] != '#' || A[i+1][j+1] != '#') {fl=false; goto imp;}
			A[i][j] = '/';
			A[i+1][j] = '\\';
			A[i][j+1] = '\\';
			A[i+1][j+1] = '/';
		}

imp:
		fprintf(fo, "Case #%d:\n", t+1);

		if (!fl)
		{
			fprintf(fo, "Impossible\n");
		}
		else
		{
			FOR(i,n)
			{
				FOR(j,m) fprintf(fo, "%c", A[i][j]);
				fprintf(fo, "\n");
			}
		}
	}

	fclose(fo);
	fclose(fi);
	return 0;
}