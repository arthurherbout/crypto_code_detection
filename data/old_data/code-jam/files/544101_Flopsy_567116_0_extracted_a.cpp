#include <cstdio>
#include <algorithm>

using namespace std;

void rotate(const int &n, char oldgrid[50][50], char newgrid[50][50]);
void gravity(const int &n, char oldgrid[50][50], char newgrid[50][50]);
bool scan(const char &c, const int &n, const int &win, char grid[50][50]);

int main()
{
	int t;
	int n;
	int k;

	char grid1[50][50];
	char grid2[50][50];
	char grid3[50][50];
	char ignore;

	scanf("%d", &t);

	for (int i = 0; i < t; i++)
	{
		scanf("%d %d", &n, &k);

		scanf("%c", &ignore);

		for (int j = 0; j < n; j++)
		{
			for (int k = 0; k < n; k++)
			{
				grid1[j][k] = '.';
				grid2[j][k] = '.';
				grid3[j][k] = '.';
			}
		}

		for (int j = 0; j < n; j++)
		{
			for (int k = 0; k < n; k++)
			{
				scanf("%c", &grid1[j][k]);
			}

			scanf("%c", &ignore);
		}

		rotate(n, grid1, grid2);

		gravity(n, grid2, grid3);

		/*
		for (int j = 0; j < n; j++)
		{
			for (int k = 0; k < n; k++)
			{
				printf("%c", grid3[j][k]);
			}

			printf("\n");
		}

		printf("\n");
		*/

		printf("Case #%d: ", i + 1);

		if (scan('R', n, k, grid3) && scan('B', n, k, grid3))
		{
			printf("Both");
		}
		else if (scan('R', n, k, grid3))
		{
			printf("Red");
		}
		else if (scan('B', n, k, grid3))
		{
			printf("Blue");
		}
		else
		{
			printf("Neither");
		}

		printf("\n");
	}
}

/*
123
456
789

741
852
963

0, 0 = 2, 0
0, 1 = 1, 0
0, 2 = 0, 0

1, 0 = 2, 1
1, 1 = 1, 1
1, 2 = 0, 1

2, 0 = 2, 2
2, 1 = 1, 2
2, 2 = 2, 0
*/

void rotate(const int &n, char oldgrid[50][50], char newgrid[50][50])
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			newgrid[i][j] = oldgrid[n - j - 1][i];
		}
	}
}

void gravity(const int &n, char oldgrid[50][50], char newgrid[50][50])
{
	for (int i = n - 1; i >= 0; i--)
	{
		for (int j = 0; j < n; j++)
		{
			if (oldgrid[i][j] == '.')
			{
				for (int k = i - 1; k >= 0; k--)
				{
					if (oldgrid[k][j] != '.')
					{
						newgrid[i][j] = oldgrid[k][j];
						oldgrid[k][j] = '.';

						break;
					}
				}
			}
			else
			{
				newgrid[i][j] = oldgrid[i][j];
			}
		}
	}	
}

bool scan(const char &c, const int &n, const int &win, char grid[50][50])
{
	bool found;

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (j + win <= n)
			{
				found = true;

				for (int k = 0; k < win; k++)
				{
					if (grid[i][j + k] != c)
					{
						found = false;
					}
				}
			}

			if (found)
			{
				return true;
			}

			//

			if (max(i, j) + win <= n)
			{
				found = true;

				for (int k = 0; k < win; k++)
				{
					if (grid[i + k][j + k] != c)
					{
						found = false;
					}
				}
			}

			if (found)
			{
				return true;
			}

			//

			if (i + win <= n)
			{
				found = true;

				for (int k = 0; k < win; k++)
				{
					if (grid[i + k][j] != c)
					{
						found = false;
					}
				}
			}

			if (found)
			{
				return true;
			}

			//

			if ((i + win <= n) && (j - win >= -1))
			{
				found = true;

				for (int k = 0; k < win; k++)
				{
					if (grid[i + k][j - k] != c)
					{
						found = false;
					}
				}
			}

			if (found)
			{
				return true;
			}
		}
	}

	return false;
}

