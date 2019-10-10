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

int A[101][101];

int F(int x, int y)
{
	if (A[x][y] == -2 || x <= 0 || y <= 0)
	{
		return 0;
	}
	if (A[x][y] == -1)
	{
		A[x][y] = (F(x-2,y-1)+F(x-1,y-2)) % 10007;
	}
	return A[x][y];
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
		int h,w,r;
		fscanf(fi, "%d%d%d",&h,&w,&r);
		int i,j;
		memset(A,0xff,sizeof(A));
		FOR(i,r){
			int v1,v2;
			fscanf(fi,"%d%d",&v1,&v2);
			A[v1][v2] = -2;
		}
		A[1][1] = 1;
		fprintf(fo, "Case #%d: %d\n", t+1, F(h,w));
	}

	fclose(fi);
	fclose(fo);
	return 0;
}