#include <iostream>
#include <vector>
#include <cstdio>
#include <algorithm>
#include <set>
#include <map>
#include <cassert>
#include <numeric>
#include <string>
#include <cstring>
#include <cmath>
using namespace std;

#ifdef LOCAL
	#define eprintf(...) fprintf(stderr, __VA_ARGS__)
#else
	#define eprintf(...) 42
#endif

typedef long long int int64;

const int maxn = 105;
vector <int> g[maxn];
int d0[maxn], d1[maxn];
int cnt[maxn];

int qu[maxn];
int l, r;

void bfs(int s, int d[maxn] )
{
	for (int i = 0; i < maxn; i++)
		d[i] = -1;
	l = r = 0;

	qu[r++] = s;
	d[s] = 0;

	while (r > l)
	{
		int v = qu[l++];
		int cd = d[v];
		for (int i = 0; i < (int) g[v].size(); i++)
		{
			int nv = g[v][i];
			if (d[nv] != -1)
				continue;
			d[nv] = cd + 1;
			qu[r++] = nv;
		}
	}
}


void solve()
{
	int n, m, k;
	scanf("%d%d%d", &n, &m, &k);
	for (int i = 0; i < maxn; i++)
	{
		cnt[i] = 0;
		g[i].clear();
	}
	for (int i = 0; i < m; i++)
	{
		int a, b;
		scanf("%d%d", &a, &b);
		g[a].push_back(b);
		g[b].push_back(a);
	}
	bfs(0, d0);
	bfs(n - 1, d1);

	for (int i = 1; i < n - 1; i++)
	{
		if (d0[i] == -1)
			continue;
		if (d0[i] + d1[i] == d0[n - 1] )
			cnt[d0[i] ]++;
	}
	int ans = d0[n - 1] + 1;
	for (int i = 1; i < d0[n - 1]; i++)
	{
		if (cnt[i] == 0)
			throw 42;
		if (cnt[i] <= k - 1)
		{
			eprintf("!\n");
			ans++;
			break;
		}
	}
	printf("%d\n", ans);
}

void multitest()
{
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	int n;
	scanf("%d", &n);
	for (int i = 1; i <= n; i++)
	{
		printf("Case #%d: ", i);
		eprintf("Case #%d .. %d\n", i, n);
		solve();
	}


}

int main(int argc, char **)
{
	if (argc == 1)
		multitest();
	else
		solve();

	return 0;
}


