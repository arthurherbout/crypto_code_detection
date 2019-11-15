#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

using namespace std;

const int md = 1000003;


int R, C;
int ans = 0;
bool ov[6][6];

string s[6];

int dfs(int r, int c)
{
	if (r >= R)
	{
		ans ++;
		return 0;
	}
	if (c >= C)
		return dfs(r + 1, 0);
	
	for (int i = 0; i <= 1; i ++)
	{
		if (s[r][c] == '|')
		{
			if (i == 0)
			{
				if (!ov[(r+R-1)%R][c])
				{
					ov[(r+R-1)%R][c] = true;
					dfs(r, c + 1);
					ov[(r+R-1)%R][c] = false;
				}
			}
			if (i == 1)
			{
				if (!ov[(r+1)%R][c])
				{
					ov[(r+1)%R][c] = true;
					dfs(r, c + 1);
					ov[(r+1)%R][c] = false;
				}
			}
		} else if (s[r][c] == '-')
		{
			if (i == 0)
			{
				if (!ov[r][(c+C-1)%C])
				{
					ov[r][(c+C-1)%C] = true;
					dfs(r, c + 1);
					ov[r][(c+C-1)%C] = false;
				}
			}
			if (i == 1)
			{
				if (!ov[r][(c+1)%C])
				{
					ov[r][(c+1)%C] = true;
					dfs(r, c + 1);
					ov[r][(c+1)%C] = false;
				}
			}
		} else if (s[r][c] == '\\')
		{
			if (i == 0)
			{
				if (!ov[(r+R-1)%R][(c+C-1)%C])
				{
					ov[(r+R-1)%R][(c+C-1)%C] = true;
					dfs(r, c + 1);
					ov[(r+R-1)%R][(c+C-1)%C] = false;
				}
			}
			if (i == 1)
			{
				if (!ov[(r+1)%R][(c+1)%C])
				{
					ov[(r+1)%R][(c+1)%C] = true;
					dfs(r, c + 1);
					ov[(r+1)%R][(c+1)%C] = false;
				}
			}
		} else if (s[r][c] == '/')
		{
			if (i == 0)
			{
				if (!ov[(r+R-1)%R][(c+1)%C])
				{
					ov[(r+R-1)%R][(c+1)%C] = true;
					dfs(r, c + 1);
					ov[(r+R-1)%R][(c+1)%C] = false;
				}
			}
			if (i == 1)
			{
				if (!ov[(r+1)%R][(c+C-1)%C])
				{
					ov[(r+1)%R][(c+C-1)%C] = true;
					dfs(r, c + 1);
					ov[(r+1)%R][(c+C-1)%C] = false;
				}
			}
		}
	}

	return 0;
}


int work()
{
	cin >> R >> C;
	memset(ov, false, sizeof(ov));
	for (int i = 0; i < R; i ++)
		cin >> s[i];
	ans = 0;
	dfs(0, 0);
	return ans % md;
}


int main()
{
	freopen("c1.txt", "r", stdin);
	freopen("c1.ans", "w", stdout);
	int T;
	cin >> T;
	for (int k = 1; k <= T; k ++)
		cout << "Case #" << k << ": " << work() << endl;
	return 0;
}