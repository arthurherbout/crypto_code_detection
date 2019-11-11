#include <iostream>
#include <string>
#include <string.h>
#include <cstring>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <bitset>
#include <queue>

using namespace std;

int n, m;
char s[110][110];
int dx[110][110], dy[110][110];

bool used[110][110];

bool check()
{
	for (int i = 0; i < n; i ++)
		for (int j = 0; j < m; j ++)
			used[i][j] = false;
	for (int i = 0; i < n; i ++)
		for (int j = 0; j < m; j ++)
			if (!used[i][j])
			{
				int x = i, y = j;
				while (true)
				{
					used[x][y] = true;
					int nx = dx[x][y], ny = dy[x][y];
					nx = (nx + n) % n, ny = (ny + m) % m;
					x = nx, y = ny;
					if (used[x][y])
					{
						if (x != i || y != j) return false;
						break;
					}
				}
			}
	return true;
}

void solve(int test)
{
	scanf("%d%d\n", &n, &m);
	for (int i = 0; i < n; i ++)
		gets(s[i]);
	int res = 0;
	for (int t = 0; t < (1 << (n * m)); t ++)
	{
		for (int i = 0; i < n; i ++)
			for (int j = 0; j < m; j ++)
			{
				int mode = (bool)(t & (1 << (i * m + j)));
				if (s[i][j] == '|')
				{
					if (mode) 
						dx[i][j] = i - 1, dy[i][j] = j; else
						dx[i][j] = i + 1, dy[i][j] = j;
				} else
				if (s[i][j] == '-')
				{
					if (mode) 
						dx[i][j] = i, dy[i][j] = j - 1; else
						dx[i][j] = i, dy[i][j] = j + 1;
				} else
				if (s[i][j] == '/')
				{
					if (mode) 
						dx[i][j] = i - 1, dy[i][j] = j + 1; else
						dx[i][j] = i + 1, dy[i][j] = j - 1;
				} else
				if (s[i][j] == '\\')
				{
					if (mode) 
						dx[i][j] = i - 1, dy[i][j] = j - 1; else
						dx[i][j] = i + 1, dy[i][j] = j + 1;
				}
			}
		res += (int)check();
	}
	printf("Case #%d: %d\n", test, res);
}

int main()
{
	freopen("input.txt","r",stdin);
	freopen("output.txt","w",stdout);
	int test;
	scanf("%d\n", &test);
	for (int i = 1; i <= test; i ++)
	{
		cerr << i << endl;
		solve(i);
	}
	return 0;
}