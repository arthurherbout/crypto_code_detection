#include <iostream>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <string>
#include <cstring>
#include <map>
#include <set>
#include <functional>
#include <cstdlib>
#include <cmath>

using namespace std;

int s2[] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192};
int vals[8192];
int teams[1024][1024];

int main()
{	// for easy
	int t, cases = 0;
	scanf("%d", &t);
	while (t--)
	{
		int p;
		scanf("%d", &p);
		fill(teams[0], teams[1024], 0);
		for (int i = 0; i < s2[p]; ++i)
		{
			scanf("%d", &vals[i]);
			vals[i] = p - vals[i];
			int j = (s2[p] + i) / 2;
			while (j > 0)
			{
				teams[j][i] = 1;
				j >>= 1;
			}
		}
		for (int i = 1; i < s2[p]; ++i)
		{	// all prices are 1. do not use.
			int q;
			scanf("%d", &q);
		}

		int res = 0;
		for (int i = 1; i < s2[p]; ++i)
		{
			int x = 0;
			for (int j = 0; j < s2[p]; ++j)
			{
				if (vals[j] == 0) continue;
				if (teams[i][j])
				{
					x = 1;
					break;
				}
			}
			if (x)
			{
				++res;
				for (int j = 0; j < s2[p]; ++j)
				{
					if (vals[j] == 0) continue;
					if (teams[i][j])
					{
						--vals[j];
					}
				}
			}
		}
		printf("Case #%d: %d\n", ++cases, res);
	}
}