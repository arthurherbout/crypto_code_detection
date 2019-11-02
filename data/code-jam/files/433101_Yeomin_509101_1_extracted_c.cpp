#include <cstdio>
#include <algorithm>

using namespace std;

int cases;
int table[1000][1000];
int prevday[1000];
long long prevR[1000];

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
		
		fill(prevday, prevday + n, -1);
		long long res = 0;
		int pt = 0;
		int preved = 0;
		for (int i = 0; i < r; ++i)
		{
			if (preved == 0)
			{
				if (prevday[pt] == -1)
				{
					prevday[pt] = i;
					prevR[pt] = res;
				}
				else
				{
					int dd = i - prevday[pt];
					long long dr = res - prevR[pt];
					preved = 1;
					int y = ((r - i) / dd);
					int daiz = dd * y;
					res += dr * y;
					i += dd * y;
					if (i == r) break;
				}
			}

			int x = upper_bound(table[pt], table[pt] + n, k) - table[pt] - 1;
			res += table[pt][x];
			pt += x + 1;
			pt %= n;
		}

		printf("Case #%d: %I64d\n", ++cases, res);
	}
}