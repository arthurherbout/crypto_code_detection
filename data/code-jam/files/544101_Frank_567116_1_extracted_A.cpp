#include<stdio.h>
int n,k;
char grid[100][100], grid1[100][100];
bool isBlue, isRed;
void readin()
{
	scanf("%d %d\n", &n, &k);

	for(int i = 0; i < n; i++)
		gets(grid[i]);
}

bool check(char grid[][100], int i, int j, int dR, int dC)
{
	if(i+dR >= n || j + dC >= n) return false;

	if(grid[i][j] == grid[i+dR][j+dC]) return 1;
	return 0;
}

void solve(char grid[][100])
{
	bool b;
	for(int i = 0; i < n; i++)
		for(int j = 0; j < n; j++)
		{
			b = 1;
			for(int p = 0; p < k; p++)
				b = b && check(grid, i, j, 1*p, 0);
			if(b && grid[i][j] == 'B') isBlue = 1;
			if(b && grid[i][j] == 'R') isRed = 1;

			b = 1;
			for(int p = 0; p < k; p++)
				b = b && check(grid, i, j, 0, 1*p);
			if(b && grid[i][j] == 'B') isBlue = 1;
			if(b && grid[i][j] == 'R') isRed = 1;

			b = 1;
			for(int p = 0; p < k; p++)
				b = b && check(grid, i, j, 1*p, 1*p);
			if(b && grid[i][j] == 'B') isBlue = 1;
			if(b && grid[i][j] == 'R') isRed = 1;

			b = 1;
			for(int p = 0; p < k; p++)
				b = b && check(grid, i, j, 1*p, -1*p);
			if(b && grid[i][j] == 'B') isBlue = 1;
			if(b && grid[i][j] == 'R') isRed = 1;

		}
}
		
void rotate()
{
	for(int i = 0; i < n; i++)
		for(int j = 0; j < n; j++)
			grid1[j][n-i-1] = grid[i][j];

	bool b;
	for(int col = 0; col < n; col++)
	{
		b = true;
		while(b)
		{
			b = false;
			for(int row = n - 2; row >= 0; row--)
				if(grid1[row][col] != '.' && grid1[row+1][col] == '.')
				{
					grid1[row+1][col] = grid1[row][col];
					grid1[row][col] = '.';
					b = true;
				}
		}

	}
}

int main()
{
#ifndef ONLINE_JUDGE
	//freopen("data.txt","r",stdin);
	freopen("A-large.in","r",stdin);
	freopen("A-large.out","w",stdout);
#endif

	int cases;
	scanf("%d\n", &cases);

	for(int i = 1; i <= cases; i++)
	{
		isBlue = isRed = false;
		printf("Case #%d: ", i);
		readin();

		solve(grid);
		rotate();
		solve(grid1);

		if(isBlue && isRed) printf("Both\n");
		else if(isBlue) printf("Blue\n");
		else if(isRed) printf("Red\n");
		else printf("Neither\n");

		
	}

	return 0;

}
