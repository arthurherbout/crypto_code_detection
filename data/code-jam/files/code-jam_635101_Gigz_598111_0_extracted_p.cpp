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

int FF[501][501];
int CC[501][501];

int C(int n, int k)
{
	if (k < 0 || k > n) return 0;

	if (CC[n][k] < 0)
	{
		if (k == 0 || k == n) CC[n][k] = 1;
		else CC[n][k] = (C(n-1, k-1) + C(n-1, k)) % 100003;
	}

	return CC[n][k];
}

int Fu(int n, int k)
{
	if (FF[n][k] < 0)
	{
		if (k == 1) 
		{
			FF[n][k] = 1;
		}
		else
		{
			FF[n][k] = 0;
			int i;
			for(i = k - 1; i >= 1; --i)
			{
				FF[n][k] = (FF[n][k] + (Fu(k, i) * C(n - k - 1, k - i - 1)) % 100003) % 100003;
			}
		}			
	}
	return FF[n][k];
}
int main()
{
	int t, tt;
	FILE *fi = fopen("a.in", "rt");
	FILE *fo = fopen("a.out", "wt");
	fscanf(fi, "%d", &tt);
	memset(FF, 0xff, sizeof(FF));
	memset(CC, 0xff, sizeof(CC));
	for (t = 0; t < tt; ++t)
	{
		int n, i;
		int res = 0;
		fscanf(fi, "%d", &n);
		for(i = 1; i <= n-1; ++i){
			res = (res + Fu(n, i)) % 100003;
		}
		fprintf(fo, "Case #%d: %d\n", t+1, res);
	}
	fclose(fi);
	fclose(fo);
	return 0;
}