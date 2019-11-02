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

int main()
{
	FILE *fi, *fo;
	fi = fopen("p1.in", "rt");
	fo = fopen("p1.out", "wt");

	int t,tt;
	fscanf(fi, "%d", &tt);
	for(t = 0; t < tt; ++t)
	{
		int i,j;
		int A,n,m;
		fscanf(fi, "%d%d%d", &n,&m, &A);
		int fl = 0;
		int x1,y1,x2,y2;
		for(x1 = 0; x1 <= n; ++x1)
			for(y1 = 0; y1 <= m; ++y1)
				for(x2 = 0; x2 <= n; ++x2)
					for(y2 = 0; y2 <= m; ++y2)
					{
						lint S = x1*y2 - x2*y1;
						if (S < 0) S = -S;
						if (S == A)
						{
							fl = 1;
							goto lbl;
						}
					}
lbl:
		if (fl)
			fprintf(fo, "Case #%d: 0 0 %d %d %d %d\n", t+1, x1, y1, x2, y2);
		else
		fprintf(fo, "Case #%d: IMPOSSIBLE\n", t+1);
	}

	fclose(fi);
	fclose(fo);
	return 0;
}