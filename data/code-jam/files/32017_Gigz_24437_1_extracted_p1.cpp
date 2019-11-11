#include <algorithm>
#include <cstdio>
#include <string>
#include <vector>
#include <cmath>
#include <iostream>
#include <map>

using namespace std;

__int64 P[3][3];

int main()
{
	FILE *fi, *fo;
	fi = fopen("p1.in", "rt");
	fo = fopen("p1.out", "wt");

	int t,tt;
	fscanf(fi, "%d", &tt);
	for(t = 0; t < tt; ++t)
	{
		__int64 n,A,B,C,D,x0,y0,M;
		fscanf(fi,"%I64d%I64d%I64d%I64d%I64d%I64d%I64d%I64d",&n,&A,&B,&C,&D,&x0,&y0,&M);
		__int64 x = x0, y = y0;
		memset(P,0,sizeof(P));
		for(int i = 0; i<n-1; ++i)
		{
			++P[x%3][y%3];
			x = (A*x + B) % M;
			y = (C*y + D) % M;
		}
		++P[x%3][y%3];

		__int64 res = 
			P[0][0] * (P[0][1]) * (P[0][2]) +
			P[1][0] * (P[1][1]) * (P[1][2]) +
			P[2][0] * (P[2][1]) * (P[2][2]) +

			P[0][0] * (P[1][0]) * (P[2][0]) +
			P[0][1] * (P[1][1]) * (P[2][1]) +
			P[0][2] * (P[1][2]) * (P[2][2]) +

			P[0][0] * (P[1][1]) * (P[2][2]) +
			P[0][0] * (P[1][2]) * (P[2][1]) +
			P[0][1] * (P[1][0]) * (P[2][2]) +
			P[0][1] * (P[1][2]) * (P[2][0]) +
			P[0][2] * (P[1][0]) * (P[2][1]) +
			P[0][2] * (P[1][1]) * (P[2][0]);
		
			if (P[0][0] >= 2) res += P[0][0] * (P[0][0] - 1) * (P[0][0] - 2) / 6;
			if (P[0][1] >= 2) res += P[0][1] * (P[0][1] - 1) * (P[0][1] - 2) / 6;
			if (P[0][2] >= 2) res += P[0][2] * (P[0][2] - 1) * (P[0][2] - 2) / 6;
			if (P[1][0] >= 2) res += P[1][0] * (P[1][0] - 1) * (P[1][0] - 2) / 6;
			if (P[1][1] >= 2) res += P[1][1] * (P[1][1] - 1) * (P[1][1] - 2) / 6;
			if (P[1][2] >= 2) res += P[1][2] * (P[1][2] - 1) * (P[1][2] - 2) / 6;
			if (P[2][0] >= 2) res += P[2][0] * (P[2][0] - 1) * (P[2][0] - 2) / 6;
			if (P[2][1] >= 2) res += P[2][1] * (P[2][1] - 1) * (P[2][1] - 2) / 6;
			if (P[2][2] >= 2) res += P[2][2] * (P[2][2] - 1) * (P[2][2] - 2) / 6;

			fprintf(fo, "Case #%d: %I64d\n", t+1, res);
	}

	fclose(fi);
	fclose(fo);
	return 0;
}