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

int n,m, res;
int A[200][50];
int B[200][200];

int U[200];
int S[200][200];
int SC[200];

int cross(int a, int b)
{
	int i;
	FOR(i, m-1)
	{
		if (A[a][i] == A[b][i] || A[a][i+1] == A[b][i+1]) return 0;
		if (A[a][i] > A[b][i] && A[a][i+1] < A[b][i+1]) return 0;
		if (A[a][i] < A[b][i] && A[a][i+1] > A[b][i+1]) return 0;
	}
	return 1;
}

void doit(int c, int a, int st)
{
	if (c > res) return;
	if (a == n)
	{
		if (res > c) res = c;
		return;
	}

	int i,j;
	SFOR(i, st, n)
	{
		if (U[i] == 0)
		{
			int fl = 1;
			FOR(j, SC[c]) if (!B[i][S[c][j]])
			{
				 fl = 0;
				 break;
			}
			if (fl)
			{
				U[i] = c;
				S[c][SC[c]++] = i;
				doit(c, a+1, i+1);
				SC[c]--;
				U[i] = 0;
			}
		}
	}

	FOR(i,n) if (U[i] == 0)
	{
		U[i] = c+1;
		S[c+1][SC[c+1]++] = i;
		doit(c+1, a+1, i+1);
		SC[c+1]--;
		U[i] = 0;
		break;
	}
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
		int i,j;
		fscanf(fi, "%d%d", &n, &m);
		FOR(i, n) FOR(j, m) fscanf(fi, "%d", &A[i][j]);
		memset(U,0,sizeof(U));
		memset(SC, 0, sizeof(SC));
		FOR(i, n) FOR(j,n) B[i][j] = cross(i,j);
		res = 1000000000;
		doit(1, 0, 0);
		fprintf(fo, "Case #%d: %d\n", t+1, res);
	}

	fclose(fi);
	fclose(fo);

	return 0;
}