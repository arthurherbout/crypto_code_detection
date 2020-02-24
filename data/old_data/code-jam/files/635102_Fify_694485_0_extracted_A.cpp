// A.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <string>
using namespace std;

int dia[55][55];

int _tmain(int argc, _TCHAR* argv[])
{
	freopen("data.txt", "r", stdin);
	freopen("data.out", "w", stdout);

	void solve();

	int t;
	cin >> t;
	for(int i = 0;i<t;i++)
	{
		solve();
	}
	return 0;
}

void solve()
{
	int k;
	cin >> k;
	for(int i = 1;i<=k;i++)
	{
		for(int j = 0;j<i;j++)
		{
			cin >> dia[i][j];
		}
	}
	for(int i = k + 1;i < 2 * k;i++)
	{
		for(int j = 0;j< 2*k - i;j++)
		{
			cin >> dia[i][j];
		}
	}

	int getLineMax(int i, int j, int length);
	int getColMax(int i, int j, int length);

	int Max = 0;
	for(int i = 1;i<=k;i++)
	{
		Max = max(Max, getLineMax(i, 0, i));
	}
	for(int i = k + 1;i< 2*k;i++)
	{
		Max = max(Max, getLineMax(i, 0, 2 * k - i));
	}

	for(int i = 1;i<=k;i++)
	{
		Max = max(Max, getColMax(i, i, (k - i) * 2 + 1));
	}

	int ans = 0;
	for(int i = 1;i<=Max;i++)
	{
		ans += i * 2 + 2 * k - 1;
	}

	//cout << ans << endl;
	static int cas = 1;
	printf("Case #%d: %d\n", cas++, ans);
}

int getLineMax(int i, int j, int length)
{
	int ans = 0;
	int x, y;
	x = j;
	y = j + length - 1;
	while(y > x)
	{
		if(dia[i][x] != dia[i][y])
		{
			ans ++;
			x ++;
		}
		else
		{
			x ++;
			y --;
		}
	}
	return ans;
}

int getColMax(int i, int j, int length)
{
	int ans = 0;
	int x, y;
	x = i;
	y = i + length - 1;
	while(y > x)
	{
		if(dia[x][j] != dia[y][j])
		{
			ans ++;
			x ++;
		}
		else
		{
			x ++;
			y --;
		}
	}

	return ans;
}