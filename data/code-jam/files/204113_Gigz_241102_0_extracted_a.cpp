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

int A[50][50];
int n;

bool valid(int r, int tr)
{
	int i;
	SFOR(i, tr+1, n) if (A[r][i]) return false;
	return true;
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
		fscanf(fi, "%d\n", &n);
		FOR(i,n) FOR(j,n)
		{
			char ch;
			do{fscanf(fi, "%c", &ch);} while ((ch != '0') && (ch != '1'));
			A[i][j] = ch - '0';
		}
		int res = 0;
		FOR(i,n)
		{
			if (valid(i, i)) continue;
			SFOR(j, i+1, n) if (valid(j,i))
			{
				int B[50];
				int k,l;
				FOR(k, n) B[k] = A[j][k];
				for(l = j-1; l >= i; l--) FOR(k,n) A[l+1][k] = A[l][k];
				FOR(k, n) A[i][k] = B[k];
				res += j-i;
				break;
			}
		}
		fprintf(fo, "Case #%d: %d\n", t+1, res);
	}

	fclose(fi);
	fclose(fo);

	return 0;
}