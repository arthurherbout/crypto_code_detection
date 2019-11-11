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

vector<pair<int, int>> P;

int main()
{
	int qt, tt;
	FILE *fi = fopen("a.in", "rt");
	FILE *fo = fopen("a.out", "wt");
	fscanf(fi, "%d", &tt);
	for (qt = 0; qt < tt; ++qt)
	{
		int n,k,b,t;
		int i,j;
		int X[1000], V[1000];
		fscanf(fi, "%d%d%d%d\n", &n, &k, &b, &t);
		FOR(i,n) fscanf(fi, "%d", &X[i]);
		FOR(i,n) fscanf(fi, "%d", &V[i]);
		FOR(i, n) P.push_back(make_pair(X[i], V[i]));
		sort(P.begin(), P.end());
		int res = 0;
		for(i = n-1; i >= 0; --i)
		{
			if (k == 0) break;
			if (b - X[i] <= t * V[i])
				--k;
			else
			{
				res += k;
			}
			if (k == 0) break;
		}
		fprintf(fo, "Case #%d: ", qt+1);
		if (k > 0) fprintf(fo, "IMPOSSIBLE\n");
		else fprintf(fo, "%d\n", res);
	}
	fclose(fi);
	fclose(fo);
	return 0;
}