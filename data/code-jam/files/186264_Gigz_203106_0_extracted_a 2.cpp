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

int wmain()
{
	FILE *fi, *fo;	
	fi = fopen("a.in", "rt");
	fo = fopen("a.out", "wt");

	
	int t, tt;
	fscanf(fi, "%d\n", &tt);
	FOR(t, tt)
	{
		char buf[30];
		fgets(buf, 30, fi);
		string s(buf);

		int n,m,i,j;
		char A[30];
		memset(A, '0', sizeof(A));
		j = 30;
		for(i=s.size()-1; i>=0; --i) if (s[i] >= '0' && s[i] <= '9') A[--j] = s[i];
		next_permutation(A, A+30);
		fprintf(fo, "Case #%d: ", t+1);
		int fl = 0;
		FOR(i,30) if (fl || A[i] != '0')
		{
			fl = 1;
			fprintf(fo, "%c", A[i]);
		}
		fprintf(fo, "\n");
	}

	fclose(fi);
	fclose(fo);

	return 0;
}