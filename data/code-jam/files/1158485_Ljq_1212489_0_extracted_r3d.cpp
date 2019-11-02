#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <math.h>

using namespace std;


string ss;

long long sqrt(long long s)
{
	long long v = floor(sqrt((double)s));
	if (v * v == s)  return v;
	return -1;
}

int dfs(int u, long long c)
{
	if (u == ss.length())
	{
		if (sqrt(c) >= 0) 
		{
			cout << ss << endl;
			return 1;
		}
		return 0;
	}
	char cc = ss[u];
	for (int i = 0; i <= 1; i ++)
	{
		if (ss[u] != '?' && i == 0)  continue;
		if (ss[u] == '?')  ss[u] = i + 48;
		if (dfs(u + 1, (c << 1) + ss[u] - 48))  return 1;
		ss[u] = cc;
	}
	return 0;
}

int work()
{
	cin >> ss;
	dfs(0, 0);
	return 0;
}

int main()
{
	freopen("d1.txt", "r", stdin);
	freopen("d1.ans", "w", stdout);
	int T;
	cin >> T;
	for (int k = 1; k <= T; k ++)
	{
		cout << "Case #" << k << ": ";
		work();
	}
	return 0;
}