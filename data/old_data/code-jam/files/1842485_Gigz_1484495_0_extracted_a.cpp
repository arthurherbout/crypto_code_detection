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

int W, L;
vector<int> R, X, Y;

bool tryit(int n, int x, int y)
{
	if (x > W || y > L) return false;
	int i;
	FOR(i, n) 
	{
		long long xx = (X[i] - x);
		xx *= xx;
		long long yy = (Y[i] - y);
		yy *= yy;
		long long rr = (R[i] + R[n]);
		rr *= rr;
		if (rr > xx + yy) return false;
	}
	return true;
}

void mainImpl(int _c)
{
	int i, j, n;
	fscanf(fi, "%d", &n);
	fscanf(fi, "%d%d", &W, &L);
	R.clear();
	X.clear();
	Y.clear();
	R.resize(n);
	X.resize(n);
	Y.resize(n);
	FOR(i, n)
	{
		fscanf(fi, "%d", &R[i]);
	}
	//sort(R.begin(), R.end());

	X[0] = 0;
	Y[0] = 0;

	SFOR(i, 1, n)
	{
		FOR(j, i)
		{
			if (tryit(i, X[j] + R[j] + R[i], Y[j]))
			{
				X[i] = X[j] + R[j] + R[i];
				Y[i] = Y[j];
				break;
			}
			if (tryit(i, X[j], Y[j] + R[j] + R[i]))
			{
				X[i] = X[j];
				Y[i] = Y[j] + R[j] + R[i];
				break;
			}
		}
	}

	fprintf(fo, "Case #%d:", _c);
	FOR(i, n) fprintf(fo, " %d %d", X[i], Y[i]);
	fprintf(fo, "\n");
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