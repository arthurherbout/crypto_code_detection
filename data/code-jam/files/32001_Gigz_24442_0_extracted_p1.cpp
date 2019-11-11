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

char S[50010];
char Q[50010];
int P[20];
int k;

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
		fscanf(fi, "%d%s",&k,&S);
		int n = strlen(S);
		FOR(i,k) P[i] = i;
		int res = n;
		do
		{
			for(i = 0; i < n; i += k)
			{
				for(j = 0; j < k; ++j)
				{
					Q[i+j] = S[i+P[j]];
				}
			}
			int cur = 1;
			FOR(i,n-1)
			{
				if (Q[i] != Q[i+1]) ++cur;
			}
			if (cur < res) res = cur;
		} while (next_permutation(P,P+k));

		fprintf(fo, "Case #%d: %d\n", t+1, res);
	}

	fclose(fi);
	fclose(fo);
	return 0;
}