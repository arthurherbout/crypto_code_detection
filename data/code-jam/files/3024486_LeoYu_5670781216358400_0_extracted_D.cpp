/*
* Problem: 
* Author: Leo Yu
* Time: 
* State: SOLVED
* Memo: 
*/
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <ctime>
#include <vector>
#include <queue>
#include <set>
#include <map>
using namespace std;
typedef long long LL;
inline int	read()
{
	int x = 0; char ch = getchar(); bool positive = 1;
	for (; ch < '0' || ch > '9'; ch = getchar())	if (ch == '-')  positive = 0;
	for (; ch >= '0' && ch <= '9'; ch = getchar())	x = x * 10 + ch - '0';
	return positive ? x : -x;
}
#define link Link
#define next Next
#define elif else if

int N;
int c[85];
bool	visit[85][85];
bool	a[85][85];

inline int	work(int x, int y)
{
	if (!x)
	{
		int ans = - 1 << 30;
		for (int i = 1; i <= N; ++ i)
		{
			int tmp = c[i];
			c[i] = 0;
			ans = max(ans, - work(y, i) + tmp);
			c[i] = tmp;
		}
		return ans;
	}
	bool	flaga = 0, flagb = 0;
	for (int i = 1; i <= N; ++ i)
		if (a[i][x] && !visit[i][x])
			flaga = 1;
	for (int i = 1; i <= N; ++ i)
		if (a[i][y] && !visit[i][y])
			flagb = 1;
	if (!flaga && !flagb) return 0;
	if (!flaga)	return - work(y, x);
	int ans = - 1 << 30;
	for (int i = 1; i <= N; ++ i)
		if (a[i][x] && !visit[i][x])
		{
			int tmp = c[i];
			c[i] = 0;
			visit[i][x] = visit[x][i] = 1;
			ans = max(ans, - work(y, i) + tmp);
			visit[i][x] = visit[x][i] = 0;
			c[i] = tmp;
		}
	return ans;
}

inline void	Main()
{
	N = read();
	for (int i = 1; i <= N; ++ i)	c[i] = read();
	memset(a, 0, sizeof(a));
	for (int i = 1; i < N; ++ i)
	{
		int j = read();
		a[i][j] = a[j][i] = 1;
	}
	memset(visit, 0, sizeof(visit));
	cout << work(0, 0) << endl;
}

int main()
{
#ifndef ONLINE_JUDGE
	freopen("d.in", "r", stdin);
	freopen("d.out", "w", stdout);
#endif

	int T = read();
	for (int t = 1; t <= T; ++ t)
	{
		printf("Case #%d: ", t);
		Main();
	}

	return 0;
}

