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

typedef long long ll;

#define INF 35184372088832LL

int s2[] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192};
int vals[8192];
int teams[1024][1024];
ll res[2048][12];
int cost[1024];

int main()
{
	int t, cases = 0;
	scanf("%d", &t);
	while (t--)
	{
		int p;
		scanf("%d", &p);
		fill(teams[0], teams[1024], 0);
		fill(res[0], res[2048], INF);
		for (int i = 0; i < s2[p]; ++i)
		{
			scanf("%d", &vals[i]);
			vals[i] = p - vals[i];
			res[s2[p] + i][vals[i]] = 0;
			int j = (s2[p] + i) / 2;
			while (j > 0)
			{
				teams[j][i] = 1;
				j >>= 1;
			}
		}
		for (int i = p - 1; i >= 0; --i)
		{
			for (int j = 0; j < s2[i]; ++j)
			{
				scanf("%d", &cost[s2[i] + j]);
			}
		}

		for (int i = s2[p] - 1; i >= 1; --i)
		{
			for (int j = 0; j <= p; ++j)
			{
				const ll& js = res[i * 2][j];
				if (js == INF) continue;
				for (int k = 0; k <= p; ++k)
				{
					const ll& ks = res[i * 2 + 1][k];
					if (ks == INF) continue;
					int tx = max(j, k);
					ll s = js + ks;
					res[i][tx] = min(res[i][tx], s);
					if (tx != 0)
						res[i][tx - 1] = min(res[i][tx - 1], s + cost[i]);
				}
			}
		}

		printf("Case #%d: %d\n", ++cases, res[1][0]);
	}
}