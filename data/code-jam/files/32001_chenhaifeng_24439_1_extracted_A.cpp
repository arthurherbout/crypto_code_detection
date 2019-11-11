#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <algorithm>
#include <iostream>
#define INF 1000000
using namespace std; 
int tcnt; 
struct tnode
{ 
	int g, c, n; 
}tree[100000];
int m, ans[100000][2]; 
int dfs(int cur, int e)
{ 
	if (ans[cur][e] != -1) 
		return ans[cur][e]; 
	if (cur * 2 + 2 > m) 
		return e == tree[cur].n ? 0 : INF; 
	int res = INF; 
	if (tree[cur].g == 1)
	{ 
		if (e == 1) 
			res <?= dfs(cur * 2 + 1, 1) + dfs(cur * 2 + 2, 1); 
		else
			res <?= dfs(cur * 2 + 1, 0) + dfs(cur * 2 + 2, 0)
					<? dfs(cur * 2 + 1, 1) + dfs(cur * 2 + 2, 0)
					<? dfs(cur * 2 + 1, 0) + dfs(cur * 2 + 2, 1); 
	}
	else
	{ 
		if (e == 0) 
			res <?= dfs(cur * 2 + 1, 0) + dfs(cur * 2 + 2, 0); 
		else
			res <?= dfs(cur * 2 + 1, 1) + dfs(cur * 2 + 2, 1)
					<? dfs(cur * 2 + 1, 0) + dfs(cur * 2 + 2, 1)
					<? dfs(cur * 2 + 1, 1) + dfs(cur * 2 + 2, 0); 
	}
	if (tree[cur].c == 1) 
	{ 
		if (tree[cur].g == 0)// ange to 1
		{ 
			if (e == 1) 
				res <?= dfs(cur * 2 + 1, 1) + dfs(cur * 2 + 2, 1) + 1; 
			else
				res <?= (dfs(cur * 2 + 1, 0) + dfs(cur * 2 + 2, 0)
					<? dfs(cur * 2 + 1, 1) + dfs(cur * 2 + 2, 0)
					<? dfs(cur * 2 + 1, 0) + dfs(cur * 2 + 2, 1)) + 1; 
		}
		else
		{ 
			if (e == 0) 
				res <?= dfs(cur * 2 + 1, 0) + dfs(cur * 2 + 2, 0) + 1; 
			else
				res <?= (dfs(cur * 2 + 1, 1) + dfs(cur * 2 + 2, 1)
					<? dfs(cur * 2 + 1, 0) + dfs(cur * 2 + 2, 1)
					<? dfs(cur * 2 + 1, 1) + dfs(cur * 2 + 2, 0)) + 1; 
		}
	}
	return ans[cur][e] = res; 
}
int go()
{
	int i, g; 
	scanf("%d%d", &m, &g); 
	for (i = 0; i < (m - 1) / 2; i++) 
	{ 
		scanf("%d%d", &tree[i].g, &tree[i].c); 
	}
	for (; i < m; i++) 
		scanf("%d", &tree[i].n); 
	memset(ans, 0xff, sizeof(ans)); 
	int tmp = dfs(0, g); 
	if (tmp < INF) 
		printf("Case #%d: %d\n", ++tcnt, tmp); 
	else
		printf("Case #%d: IMPOSSIBLE\n", ++tcnt); 
}
int main()
{ 
	freopen("A-large.in", "r", stdin); 
	freopen("aoutl.txt", "w", stdout); 
	int T; 
	scanf("%d", &T); 
	tcnt = 0; 
	while (T--) 
		go(); 
	return 0; 
}
