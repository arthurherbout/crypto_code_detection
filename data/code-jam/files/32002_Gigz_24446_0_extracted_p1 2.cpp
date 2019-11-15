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

char A[100][100];

int F[10][2000000];
int T[20], TT[20];

int main()
{
	FILE *fi, *fo;
	fi = fopen("p1.in", "rt");
	fo = fopen("p1.out", "wt");

	int t,tt;
	fscanf(fi, "%d", &tt);
	for(t = 0; t < tt; ++t)
	{
		int n,m;
		fscanf(fi, "%d%d",&n,&m);
				int i,j,k;

		FOR(i,n) FOR(j,m){
			char ch;
			do { fscanf(fi,"%c",&ch);} while (! (ch == '.' || ch =='x'));
			A[i][j] = ch;
		}


		memset(F,0,sizeof(F));

		FOR(i, 1<<m)
		{
			int ones = 0;
			int fl = 1;
			memset(T,0,sizeof(T));
			FOR(j,m) if (i & (1<<j))
			{
				++ones;
				if (T[j] || A[n-1][j] == 'x')
				{
					fl = 0;
					break;
				}
				T[j + 1] = 1;
			}
			if (fl) F[n-1][i] = ones;
		}

		for(k = n-2; k >= 0; --k)
		{
			FOR(i, 1<<m)
			{			
				int ones = 0;
				int fl = 1;
				memset(T,0,sizeof(T));
				FOR(j,m) if (i & (1<<j))
				{
					++ones;
					if (T[j] || A[k][j] == 'x')
					{
						fl = 0;
						break;
					}
					T[j + 1] = 1;
				}
				if (fl)
				{
					memcpy(TT,T,sizeof(T));
					int l;
					FOR(l, 1<<m)
					{
						int fl2 = 1;
						memcpy(T,TT,sizeof(T));
						FOR(j,m) if (l & (1<<j))
						{
							if (T[j] || T[j + 2] || A[k + 1][j] == 'x')
							{
								fl2 = 0;
								break;
							}
							T[j + 1] = 1;
						}
						if (fl2)
						{
							if (F[k][i] < F[k+1][l] + ones) F[k][i] = F[k+1][l] + ones;
						}
					}
				}
			}
		}

		int res = 0;

		FOR(i, 1<<m) if (F[0][i] > res) res = F[0][i];

		fprintf(fo, "Case #%d: %d\n", t+1, res);
	}

	fclose(fi);
	fclose(fo);
	return 0;
}