#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cstring>

using namespace std;

int parent[10000];
int sinkName[10000];
int table[100][100];
bool calced[100][100];

int getTree(int x)
{
	if (parent[x] == -1) return x;
	return parent[x] = getTree(parent[x]);
}

void unionTree(int x, int y)
{
	x = getTree(x); y = getTree(y);
	if (x == y) return;
	parent[x] = y;
}

int main()
{
	int t, cases = 0;
	scanf("%d", &t);
	while (t--)
	{
		int n, m;
		scanf("%d%d", &n, &m);
		fill(parent, parent + 10000, -1);
		fill(sinkName, sinkName + 10000, -1);
		fill(calced[0], calced[n], false);
		for (int i = 0; i < n; ++i)
			for (int j = 0; j < m; ++j)
				scanf("%d", &table[i][j]);

		for (int i = 0; i < n; ++i)
		{
			for (int j = 0; j < m; ++j)
			{
				int x = i, y = j;
				const static int arr[4][2] = {{-1, 0}, {0, -1}, {0, 1}, {1, 0}};
				while(true)
				{
					if (calced[x][y]) break;
					calced[x][y] = true;
					int cx = x, cy = y;
					int cz = table[x][y];
					for (int k = 0; k < 4; ++k)
					{
						int x2 = x + arr[k][0], y2 = y + arr[k][1];
						if (x2 < 0 || x2 >= n || y2 < 0 || y2 >= m) continue;
						if (cz > table[x2][y2])
						{
							cz = table[x2][y2];
							cx = x2; cy = y2;
						}
					}
					if (cz == table[x][y]) break;
					unionTree(x * m + y, cx * m + cy);
					x = cx; y = cy;
				}
			}
		}
		char name = 'a';
		printf("Case #%d:\n", ++cases);
		for (int i = 0; i < n; ++i)
		{
			for (int j = 0; j < m; ++j)
			{
				if (j != 0) printf(" ");
				int root = getTree(i * m + j);
				if (sinkName[root] == -1) sinkName[root] = name++;
				printf("%c", sinkName[root]);
			}
			printf("\n");
		}
	}
}

