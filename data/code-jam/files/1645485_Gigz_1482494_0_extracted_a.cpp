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

int A[1001];
int B[1001];
int V[1001];

void mainImpl(int _c)
{
	int n, i, j;
	fscanf(fi, "%d", &n);
	FOR(i, n) fscanf(fi, "%d%d", &A[i], &B[i]);
	memset(V, 0, sizeof(V));

	int res = 0, cur = 0, cnt = 0;
	while (cnt != n)
	{
		int mn = 1000000, mx = -1, mnp, mxp;
		FOR(j, n)
		{
			if (V[j] == 0 && cur >= A[j] && mx < B[j]) {mx = B[j]; mxp = j;}
			if (V[j] != 2 && mn > B[j]) {mn = B[j]; mnp = j;}
		}

		if (mx < 0 && cur < mn)
		{
			fprintf(fo, "Case #%d: Too Bad\n", _c);
			return;
		}

		if (cur >= mn)
		{
			cur += 2 - V[mnp];
			V[mnp] = 2;
			++cnt;
			++res;
		}
		else
		{
			V[mxp] = 1;
			++res;
			++cur;
		}
	}
	fprintf(fo, "Case #%d: %d\n", _c, res);
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