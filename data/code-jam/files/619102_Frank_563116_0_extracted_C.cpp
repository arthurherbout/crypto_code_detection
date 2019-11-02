#include<stdio.h>

#include<string.h>

#define MAX 110

char hex[][5] ={"0000", "0001", "0010", "0011","0100", "0101", "0110", "0111", "1000",
"1001", "1010", "1011", "1100", "1101", "1110", "1111"};
char grid[1000][1000];
int Row, Col;
int count[1000][1000];
int ans[1000];

void addGrid(int row, int col, char c)
{
	int cc;
	if(c >= '0' && c <='9') cc = c-'0';
	if(c=='A') cc = 10;
	if(c=='B') cc = 11;
	if(c=='C') cc = 12;
	if(c=='D') cc = 13;
	if(c=='E') cc = 14;
	if(c=='F') cc = 15;

	for(int i = 0; i< 4; i++)
	{
		grid[row][col*4+i] = hex[cc][i];
	}
}


void readin()
{
	char c;
	scanf("%d %d\n", &Row, &Col);

	for(int i = 0; i < Row; i++)
	{
		for(int j =0; j < Col/4; j++)
		{
			scanf("%c", &c);
			addGrid(i, j, c);
		}
		scanf("\n");
	}
}

bool check(int row, int col, int size)
{
	if(row + size > Row || col+size > Col) return 0;

	for(int i = row; i < row+size-1; i++)
	{
		for(int j = col; j < col+size; j++)
			if(grid[i][j] == grid[i+1][j] || grid[i+1][j] == 'x') return 0;
	}

	for(int i = row; i < row+size; i++)
	{
		for(int j = col; j < col+size-1; j++)
			if(grid[i][j] == grid[i][j+1] || grid[i][j+1] == 'x') return 0;

	}


	return 1;
}

void procCount()
{
	int size;
	for(int i = 0; i < Row; i++)
		for(int j =0; j < Col; j++)
		{
			if(grid[i][j] == 'x') continue;
			size = 2;

			while(check(i, j, size)) size++;
			count[i][j] = size - 1;
		}

}
void solve()
{

	procCount();
	
	for(int i = 0; i < 1000; i++)
		ans[i] = 0;

	bool flag = true;
	int max = 0, row, col, out = 0;
	while(flag)
	{
		flag = max = 0 ;
		for(int i = 0; i < Row; i++)
		{
			for(int j =0; j < Col; j++)
				if(count[i][j] > max) 
				{
					max = count[i][j];
					row = i;
					col = j;
					flag = true;
				}
		}
		if(!flag) break;
		if(ans[max] == 0) out++;
		ans[max]++;
		for(int i = row; i < row+max; i++)
		{
			for(int j =col; j < col+max; j++)
			{
				count[i][j] = 0;
				grid[i][j] = 'x';
			}
		}
		procCount();
	}

	printf("%d\n", out);

	for(int i = 1000; i >= 0; i--)
		if(ans[i] > 0)
			printf("%d %d\n", i, ans[i]);



	

}
int main()
{
#ifndef ONLINE_JUDGE
	//freopen("data.txt","r",stdin);
	freopen("C-small-attempt0.in","r",stdin);
	freopen("C-small-attempt0.out","w",stdout);
#endif

	int cases;
	scanf("%d\n", &cases);

	for(int i = 1; i <= cases; i++)
	{
		printf("Case #%d: ", i);
		readin();
		solve();
	}

	return 0;

}
