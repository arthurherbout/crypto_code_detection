#include <cstdio>
#include <algorithm>
#include <vector>
#include <cmath>

using namespace std;

#define FOR(i,n) for(i = 0; i < (n); ++i)
#define SFOR(i,m,n) for(i = (m); i < (n); ++i)
#define pb push_back

int A[2][1<<11];

int main()
{
	FILE *fi = fopen("p.in", "rt");
	FILE *fo = fopen("p.out", "wt");
	int t,tt;
	fscanf(fi,"%d",&tt);
	FOR(t,tt)
	{
		int n,k,p;
		int i,j,a,b;
		fscanf(fi,"%d%d%d",&n,&k,&p);
		memset(A,0,sizeof(A));
		int x = 0;
		FOR(i,k) x += 1<<i;
		A[(k-1)%2][x] = 1;
		for(j = k; j < n; ++j)
		{
			memset(A[j%2], 0, sizeof(A[j%2]));
			FOR(i, 1<<p) if (A[(j+1)%2][i] > 0)
			{
				FOR(a,p) if (i & (1<<a))
				{
					if ((a != p - 1) && (i & (1 << (p-1)))) continue;
					x = i;
					x -= (1<<a);
					x <<= 1;
					x++;
					A[j%2][x] = (A[j%2][x] + A[(j+1)%2][i]) % 30031;
				}
			}
		}
		x = 0;
		FOR(i,k) x += 1<<i;
		fprintf(fo,"Case #%d: %d\n",t+1, A[(n-1)%2][x]);
	}
	fclose(fi);
	fclose(fo);
	return 0;
}