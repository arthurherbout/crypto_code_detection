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
vector < int > g[600];
vector < int > ng[600];
int d[600], used[600], d2[600];

int dp[600];

queue < int > q;
vector < int > order;

void dfs(int v)
{
	if (used[v]) return ;
	used[v] = true;

	for (int i = 0; i < ng[v].size(); i ++)
		dfs(ng[v][i]);
	order.push_back(v);
}


void solve(int test)
{
	scanf("%d%d", &n, &m);
	for (int i = 0; i < n; i ++)
		g[i].clear(),
		ng[i].clear();
	for (int i = 0; i < m; i ++)
	{
		int u, v;
		scanf("%d,%d", &u, &v);
		g[u].push_back(v);
		g[v].push_back(u);
	}

	for (int i = 0; i < n; i ++)
		d[i] = d2[i] = 1000000;
	d[0] = d2[1] = 0;
	

	q.push(0);
	while (!q.empty())
	{
		int v = q.front(); q.pop();
		for (int i = 0; i < g[v].size(); i ++)
			if (d[g[v][i]] > d[v] + 1)
			{
				d[g[v][i]] = d[v] + 1;
				q.push(g[v][i]);
			}
	}

	q.push(1);
	while (!q.empty())
	{
		int v = q.front(); q.pop();
		for (int i = 0; i < g[v].size(); i ++)
			if (d2[g[v][i]] > d2[v] + 1)
			{
				d2[g[v][i]] = d2[v] + 1;
				q.push(g[v][i]);
			}
	}

	for (int i = 0; i < n; i ++)
		for (int j = 0; j < g[i].size(); j ++)
			if (d[g[i][j]] == d[i] + 1)
				ng[i].push_back(g[i][j]);

	for (int i = 0; i < n; i ++)
		used[i] = false;
	order.clear();
	dfs(0);

	int start = 0;
	for (int i = 0; i < order.size(); i ++)
		if (order[i] == 1) start = i;
	for (int i = 0; i < n; i ++)
		dp[i] = 0;
	dp[1] = 1;
	for (int i = start + 1; i < order.size(); i ++)
	{
		int v = order[i];
		dp[v] = 0;
		/*for (int j = 0; j < ng[v].size(); j ++)
		{
			int u = ng[v][j];
			if (d2[u] + d[v] + 1 != d[1]) dp[v] ++;
		}*/

		int add = 0;
		for (int j = 0; j < ng[v].size(); j ++)
		{
			int u = ng[v][j];
			if (d2[u] + d[v] + 1 == d[1]) add = max(add, dp[u]);
		}
		dp[v] = ng[v].size() - 1 + add;
	}

	printf("Case #%d: %d %d\n", test, d[1] - 1, dp[0]);
}

int main()
{
	freopen("input.txt","r",stdin);
	freopen("output.txt","w",stdout);
	int test;
	scanf("%d\n", &test);
	for (int i = 1; i <= test; i ++)
		solve(i);
	return 0;
}