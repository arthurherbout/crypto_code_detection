#include <cstdio>
#include <algorithm>

using namespace std;

int cases;
int table[1000][1000];

int main()
{
	int t;
	scanf("%d", &t);
	while(t--)
	{
		int r, k;
		int n, g[10000];
		scanf("%d%d%d", &r, &k, &n);
		for (int i = 0; i < n; ++i)
			scanf("%d", &g[i]);
		for (int i = 0; i < n; ++i)
		{
			table[i][0] = g[i];
			for (int j = 1; j < n; ++j)
			{
				int x = (i + j) % n;
				table[i][j] = table[i][j - 1] + g[x];
			}
		}
		
		long long res = 0;
		int pt = 0;
		for (int i = 0; i < r; ++i)
		{
			int st = pt;
			int avail = k;
			while (avail >= g[pt])
			{
				avail -= g[pt];
				res += g[pt];
				++pt;
				if (pt == n) pt = 0;
				if (pt == st) break;
			}
			/*
			if (pt == 0)
			{
				int y = r / (i + 1);
				res *= y;
				i += (i + 1) * y;
			}
			*/
		}

		printf("Case #%d: %I64d\n", ++cases, res);
	}
}