#include <cassert>
#include <cstring>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <set>
#include <map>
#include <queue>
#include <stack>

using namespace std;

const int N = 22;
int n;
char s[N];

double mem[1 << N];
bool vis[1 << N];

double f(const int mask)
{
	double & r = mem[mask];

	if(!vis[mask])
	{
		vis[mask] = true;
		r = 0.0;

		for(int i = 0; i < n; i++)
		{
			int cost = n;

			int j = i;
			while((mask & (1 << j)) != 0)
			{
				j = (j + 1) % n;
				cost--;
			}

			r += (1.0 / n) * (cost + f(mask | (1 << j)));
		}
	}

	return r;
}

double solve()
{
	memset(vis, 0, sizeof(vis));
	mem[(1 << n) - 1] = 0.0;
	vis[(1 << n) - 1] = true;

	int mask = 0;
	for(int i = 0; i < n; i++)
		if(s[i] == 'X')
			mask += (1 << i);

	return f(mask);
}

int main()
{
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	int t;
	scanf("%i\n", &t);

	for(int ti = 1; ti <= t; ti++)
	{
		scanf("%s\n", s);
		n = (int)strlen(s);

		cout << "Case #" << ti << ": ";
		printf("%0.15lf", solve());
		cout << "\n";
	}

	return 0;
}
