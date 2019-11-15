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

int A[400][400], B[400][400];

int main()
{
	int t, tt;
	FILE *fi = fopen("a.in", "rt");
	FILE *fo = fopen("a.out", "wt");
	fscanf(fi, "%d", &tt);
	for (t = 0; t < tt; ++t)
	{
		int n, res = 1000000000;
		fscanf(fi, "%d", &n);
		int i,j;
		memset(A, -1, sizeof(A));
		SFOR(i,0,n)
		{
			FOR(j, i / 2 + 1) fscanf(fi, "%d", &A[201 - n + i][200 - i + j*2]);
			FOR(j, i / 2 + i%2) fscanf(fi, "%d", &A[201 - n + i][201 + j*2 + (i+1)%2]);
		}
		for(i = n - 2; i >= 0; --i)
		{
			FOR(j, i / 2 + 1) fscanf(fi, "%d", &A[199 + n - i][200 - i + j*2]);
			FOR(j, i / 2 + i%2) fscanf(fi, "%d", &A[199 + n - i][201 + j*2 + (i+1)%2]);
		}

		int cx, cy, ii, jj;
		SFOR(ii, -2*n+1, n*2)
			SFOR(jj, -(2*n - 1-abs(ii)), 2*n-abs(ii))
		{
			cx = 200 + ii;
			cy = 200 + jj;
			int cur = 0;
			FOR(i, 2*n+3) 
			{
				FOR(j, 2*n+3)
				{
					set<int> S;
					S.insert(A[cx - i][cy - j]);
					S.insert(A[cx + i][cy - j]);
					S.insert(A[cx - i][cy + j]);
					S.insert(A[cx + i][cy + j]);
					if (S.size() == 1) continue;
					if (S.size() > 2) { cur = 1; break;}
					if (S.find(-1) != S.end()) continue;
					else { cur = 1; break;}
				}
				if (cur == 1) break;
			}
			if (!cur)
			{
				int m = abs(ii) + abs(jj) + n;
				//cout<<i<<" "<<j<<endl;
				if (m*m - n*n < res) res = m*m - n*n;
			}
		}
		
		fprintf(fo, "Case #%d: %d\n", t+1, res);
	}
	fclose(fi);
	fclose(fo);
	return 0;
}