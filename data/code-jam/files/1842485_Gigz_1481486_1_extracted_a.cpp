#include <vector>
#include <iostream>
#include <cstring>
#include <cstdio>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>

using namespace std;

#define FOR(i,n) for((i) = 0; (i) < (n); ++(i))
#define SFOR(i,m,n) for((i) = (m); (i) < (n); ++(i))

FILE *fi, *fo;

vector<int> D;
vector<int> L;
vector<int> R;

void mainImpl(int _c)
{
	int i, j, n;
	fscanf(fi, "%d", &n);
	int v1, v2;
	D.clear();
	L.clear();
	R.clear();
	FOR(i, n)
	{
		fscanf(fi, "%d%d", &v1, &v2);
		D.push_back(v1);
		L.push_back(v2);
		R.push_back(0);
	}
	int TD;
	fscanf(fi, "%d", &TD);

	R[0] = D[0];
	FOR(i, n)
	{
		if (R[i] + D[i] >= TD)
		{
			fprintf(fo, "Case #%d: YES\n", _c);
			return;
		}
		SFOR(j, i+1, n) if (R[i] + D[i] >= D[j]) R[j] = max(R[j], min(L[j], D[j] - D[i]));
	}
	fprintf(fo, "Case #%d: NO\n", _c);
}


int main()
{
	fi = fopen("a.in", "rt");
	fo = fopen("a.out", "wt");

	int t, tt;
	fscanf(fi, "%d", &tt);

	FOR(t, tt) mainImpl(t + 1);

	fclose(fo);
	fclose(fi);
	return 0;
}