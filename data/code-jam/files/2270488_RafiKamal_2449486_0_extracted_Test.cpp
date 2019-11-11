#include <cstdio>
#include <cctype>
#include <algorithm>
#include <cstring>
#include <iostream>
#include <cmath>
#include <stack>
#include <string>     
#include <map>
#include <set>
#include <list>
#include <queue>
#include <utility>

#define SIZE 5005
#define INT_MAX 2000000000
#define MOD 20071027
#define clr(a) memset(a, 0, sizeof a)
#define reset(a) memset(a, -1, sizeof a)

#define BOUNDARY(i, j) ((i >= 0 && i < row) && (j >= 0 && j < column))

#define max3(x, y, z) max(max(x, y), max(y, z)) 

using namespace std;

int n, m, row, column;

int grid[1005][1005];
bool solved[1005][1005];

bool test(int i, int j, int val)
{
	int k;
	for(k = 0; k < m; k++)
		if(grid[i][k] > val) break;
	if(k == m)
	{
		for(k = 0; k < m; k++)
			if(grid[i][k] == val) solved[i][k] = true;
		return true;
	}

	for(k = 0; k < n; k++)
		if(grid[k][j] > val) break;
	if(k == n)
	{
		for(k = 0; k < n; k++)
			if(grid[k][j] == val) solved[k][j] = true;
		return true;
	}

	return false;
}

int main()
{	
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
	
	int i, j, k, l;
	int x, y;
	int tc, t;
	int res = 0;
	
	scanf("%d", &tc);

	for(t = 1; t <= tc; t++)
	{
		scanf("%d %d", &n, &m);
		printf("Case #%d: ", t);

		clr(solved);

		bool flag = true;
		for(i = 0; i < n; i++)
			for(j = 0; j < m; j++)
				scanf("%d", &grid[i][j]);

		for(i = 0; i < n && flag; i++)
			for(j = 0; j < m && flag; j++)
				if(!solved[i][j] && !test(i, j, grid[i][j]))
					flag = false;
				
		if(flag)
			puts("YES");
		else
			puts("NO");				
	}
			
	return 0;
}