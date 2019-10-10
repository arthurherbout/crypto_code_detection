#include <cstdio>
#include <algorithm>
using namespace std;
#define N 2000 + 5

int Case, n, m, tot, Fa[N], Head[N], Dep[N], Next[N], Ans[N];
bool Flag[N];

struct Edge
{
	int next, node, w;
	bool tree;
}h[N << 1];

inline void addedge(int u, int v)
{
	h[++ tot].next = Head[u], Head[u] = tot;
	h[tot].node = v, h[tot].w = h[tot].tree = 0;
}

void dfs(int z, int fa)
{
	Flag[z] = 1;
	for (int i = Head[z]; i; i = h[i].next)
	{
		int d = h[i].node;
		if (d == fa || Flag[d]) continue ;
		h[i].tree = 1;
		Fa[d] = i ^ 1, Dep[d] = Dep[z] + 1;
		dfs(d, z);
	}
}

void dfs(int z)
{
	for (int i = Head[z]; i; i = h[i].next)
	{
		int d = h[i].node;
		if (Dep[d] < Dep[z] && !h[i ^ 1].tree)
		{
			h[i].w ++;
			for (int x = z; x != d; x = h[Fa[x]].node)
				h[Fa[x] ^ 1].w ++;
		}
	}
	for (int i = Head[z]; i; i = h[i].next)
	{
		if (!h[i].tree) continue ;
		int d = h[i].node;
		dfs(d);
	}
}

int main()
{
	scanf("%d", &Case);
	for (int Test = 1; Test <= Case; Test ++)
	{
		scanf("%d%d", &n, &m);
		tot = 1;
		for (int i = 1; i <= n; i ++)
			Head[i] = Flag[i] = 0;
		for (int i = 1, u, v; i <= m; i ++)
		{
			scanf("%d%d", &u, &v);
			addedge(u, v);
			addedge(v, u);
		}
		for (int i = 1; i <= n; i ++)
			if (!Flag[i])
			{
				dfs(i, Dep[i] = Fa[i] = 0);
				dfs(i);
			}
		bool ok = 1;
		for (int i = 1; ok && i <= m; i ++)
		{
			Ans[i] = h[i << 1].w - h[i << 1 | 1].w;
			if (!Ans[i]) ok = 0;
		}
		printf("Case #%d: ", Test);
		if (!ok) puts("IMPOSSIBLE");
		else for (int i = 1; i <= m; i ++)
			printf("%d%c", Ans[i], i == m ? '\n' : ' ');
	}
	return 0;
}
