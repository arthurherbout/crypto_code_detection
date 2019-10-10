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

int table[2][250][250];

int main()
{	// for c-small
	int t, cases = 0;
	scanf("%d", &t);
	while (t--)
	{
		int n, x1, y1, x2, y2;
		scanf("%d", &n);
		fill(table[0][0], table[2][0], 0);
		for (int i = 0; i < n; ++i)
		{
			scanf("%d%d%d%d", &x1, &y1, &x2, &y2);
			for (int i = x1; i <= x2; ++i)
				for (int j = y1; j <= y2; ++j)
				{
					table[0][i][j] = 1;
				}
		}
		int cur = 0, nex = 1;
		int res = 0;
		while (n != 0)
		{
			int bacs = 0;
			for (int i = 1; i <= 250; ++i)
				for (int j = 1; j <= 250; ++j)
				{
					if (table[cur][i][j] == 1)
					{
						if (table[cur][i - 1][j] == 1 || table[cur][i][j - 1] == 1)
						{
							table[nex][i][j] = 1;
							++bacs;
						}
						else
							table[nex][i][j] = 0;
					}
					else
					{
						if (table[cur][i - 1][j] == 1 && table[cur][i][j - 1] == 1)
						{
							table[nex][i][j] = 1;
							++bacs;
						}
						else
							table[nex][i][j] = 0;
					}
				}
			++res;
			if (bacs == 0) break;
			swap(cur, nex);
			fill(table[nex][0], table[nex][250], 0);
		}
			
		printf("Case #%d: %d\n", ++cases, res);
	}
}