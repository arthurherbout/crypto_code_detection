#include <cstdio>
#include <string>
#include <vector>
#include <cmath>
#include <iostream>
#include <map>

using namespace std;

int FF[2000][200];
int A[2000];
map<string, int> M;
int S;

int F(int q, int s)
{
	if (FF[q][s] < 0)
	{
		if (q == 0) FF[q][s] = 0;
		else
		{
			FF[q][s] = 1000000000;
			if (A[q-1] != s)
			{
				for(int i = 0; i < S; ++i)
				{
					if (i != s) FF[q][s] = min(FF[q][s], F(q-1, i) + 1);
					else FF[q][s] = min(FF[q][s], F(q-1, i));
				}
			}
		}
	}
	return FF[q][s];
}

int main()
{
	FILE *fi, *fo;
	fi = fopen("p1.in", "rt");
	fo = fopen("p1.out", "wt");

	int n,i,j;
	fscanf(fi, "%d", &n);
	for(i = 0; i < n; ++i)
	{
		M.clear();
		int s,q;
		fscanf(fi, "%d\n", &s);
		S = s;
		for (j=0;j<s;++j)
		{
			char buf[1000];
			fgets(buf, 1000, fi);
			M[string(buf)] = j;
		}
		fscanf(fi, "%d\n", &q);
		for (j=0;j<q;++j)
		{
			char buf[1000];
			fgets(buf, 1000, fi);
			A[j] = M[string(buf)];
		}
		
		int res = 1000000000;
		memset(FF, 0xFF, sizeof(FF));
		for (j=0;j<s;++j)
		{
			res = min(res, F(q,j));
		}

		fprintf(fo, "Case #%d: %d\n", i+1, res);
	}


	fclose(fi);
	fclose(fo);
	return 0;
}