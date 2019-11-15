#include <iostream>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <string>
#include <list>
#include <set>
#include <map>
#include <functional>

using namespace std;

int cases;
char table[100][100];
int table2[100][100];
int arr[4][2] = {{0, 1}, {1, 0}, {1, 1}, {-1, 1}};

int main()
{
	int t;
	scanf("%d", &t);
	while (t--)
	{
		int n, k;
		scanf("%d%d", &n, &k);
		for (int i = 0; i < n; ++i)
			scanf("%s", table[i]);
		fill(table2[0], table2[n], 0);
		for (int i = 0; i < n; ++i)
		{
			int x = n - 1;
			for (int j = n - 1; j >= 0; --j)
			{
				if (table[i][j] == '.') continue;
				if (table[i][j] == 'R')
					table2[i][x--] = 1;
				else if (table[i][j] == 'B')
					table2[i][x--] = 2;
			}
		}
		/*
		for (int i = 0; i < n; ++i)
		{
			for (int j = 0; j < n; ++j)
				printf("%d", table2[i][j]);
			printf("\n");
		}
		*/
		int r = 0, b = 0;
		for (int i = 0; i < n; ++i)
		{
			for (int j = 0; j < n; ++j)
			{
				if (table2[i][j] == 0) continue;
				for (int l = 0; l < 4; ++l)
				{
					int x = i, y = j;
					int q;
					int x2 = i - arr[l][0], y2 = j - arr[l][1];
					if (x2 >= 0 && x2 < n && y2 >= 0 && y2 < n)
					{
						if (table2[i][j] == table2[x2][y2]) continue;
					}
					for (q = 1; q < k; ++q)
					{
						x += arr[l][0]; y += arr[l][1];
						//printf("l: %d, arr: {%d, %d}, x: %d, y:%d\n", l, arr[l][0], arr[l][1], x, y);
						if (x < 0 || x >= n || y < 0 || y >= n) break;
						if (table2[i][j] != table2[x][y]) break;
					}
					//printf("i: %d, j: %d, l: %d, q: %d, k: %d, r: %d, b: %d\n", i, j, l, q, k, r, b);
					if (q == k)
					{
						if (table2[i][j] == 1) r = 1;
						else if (table2[i][j] == 2) b = 1;
					}
				}
			}
		}
		printf("Case #%d: ", ++cases);
		if (r == 0 && b == 0)
			printf("Neither\n");
		else if (r == 1 && b == 0)
			printf("Red\n");
		else if (r == 0 && b == 1)
			printf("Blue\n");
		else
			printf("Both\n");
	}
}