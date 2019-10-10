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

int LCD(int a, int b)
{
	if (b == 0) return a;
	return LCD(b, a%b);
}

int LCM(int a, int b)
{
	return a / LCD(a,b) * b;
}

int main()
{
	FILE *fi = fopen("a.txt", "rt");
	FILE *fo = fopen("a.out", "wt");

	int t,tt;

	fscanf(fi, "%d", &tt);
	FOR(t,tt)
	{
		int i,n,l,h,j;
		int A[1000];
		fscanf(fi, "%d%d%d", &n, &l, &h);
		FOR(i,n) fscanf(fi, "%d", &A[i]);

		int res = -1;
		SFOR(i, l, h+1)
		{
			int fl = 1;
			FOR(j, n) if (i % A[j] != 0 && A[j] % i != 0) {fl = 0; break;}
			if (fl) {res = i; break;}
		}

		fprintf(fo, "Case #%d: ", t+1);
		if (res < 0) fprintf(fo, "NO\n");
		else fprintf(fo, "%d\n", res);
	}

	fclose(fo);
	fclose(fi);
	return 0;
}