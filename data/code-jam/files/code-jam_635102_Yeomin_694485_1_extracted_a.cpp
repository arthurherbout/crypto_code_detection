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

int sz[500];
int table[500][500];
int k;

bool chk(int cx, int cy)
{
	int x = 240;
	for (int i = 1; i <= k; ++i)
	{
		int y = 240 - i;
		for (int j = 1; j <= i; ++j)
		{
			int px = x + 2 * (cx - x);
			int py = y + 2 * (cy - y);
			if (table[px][y] != -1 && table[px][y] != table[x][y])
				return false;
			if (table[x][py] != -1 && table[x][py] != table[x][y])
				return false;
			y += 2;
		}
		++x;
	}
	for (int i = k - 1; i > 0; --i)
	{
		int y = 240 - i;
		for (int j = 1; j <= i; ++j)
		{
			int px = x + 2 * (cx - x);
			int py = y + 2 * (cy - y);
			if (table[px][y] != -1 && table[px][y] != table[x][y])
				return false;
			if (table[x][py] != -1 && table[x][py] != table[x][y])
				return false;
			y += 2;
		}
		++x;
	}

	return true;
}

int main()
{
	int t, cases = 0;
	scanf("%d", &t);
	for (int i = 1; i < 500; ++i)
	{
		sz[i] = ((i * (i + 1)) + (i * (i - 1))) / 2;
	}
	while (t--)
	{
		fill(table[0], table[500], -1);
		scanf("%d", &k);
		int res = 10000000;
		int x = 240;
		for (int i = 1; i <= k; ++i)
		{
			int y = 240 - i;
			for (int j = 1; j <= i; ++j)
			{
				scanf("%d", &table[x][y]);
				y += 2;
			}
			++x;
		}
		for (int i = k - 1; i > 0; --i)
		{
			int y = 240 - i;
			for (int j = 1; j <= i; ++j)
			{
				scanf("%d", &table[x][y]);
				y += 2;
			}
			++x;
		}

		int cx = 240 + k - 1, cy = 239;
		x = 240 - k;
		for (int i = 1; i <= k * 2; ++i)
		{
			int y = 240 - i;
			int z = i * 2 - 1;
			for (int j = 1; j <= z; ++j)
			{	// x, y is center
				int nk = abs(cx - x) + abs(cy - y) + k;
				if (chk(x, y))
				{
					res = min(res, sz[nk]);
				}
				++y;			
			}
			++x;
		}
		for (int i = k * 2 - 1; i > 0; --i)
		{
			int y = 240 - i;
			int z = i * 2 - 1;
			for (int j = 1; j <= z; ++j)
			{	// x, y is center
				int nk = abs(cx - x) + abs(cy - y) + k;
				if (chk(x, y))
				{
					res = min(res, sz[nk]);
				}
				++y;			
			}
			++x;
		}

		printf("Case #%d: %d\n", ++cases, res - sz[k]);
	}
}