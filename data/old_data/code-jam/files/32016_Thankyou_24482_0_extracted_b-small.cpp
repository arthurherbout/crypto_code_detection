#include <stdio.h>
#include <string.h>
#include <algorithm>
using namespace std;

const int MAXN = 10;
const int MAXM = 100;

int n, m;
int adj[MAXM][MAXN];

void solve()
{
	int ans[MAXN];
	int one = n+1;
	for (int state=0; state<(1<<n); state++)
	{
		int tmp[MAXN];
		for (int i=0, j=state; i<n; i++)
		{
			tmp[i] = j%2;
			j /= 2;
		}	
		bool ok = true;
		for (int i=0; i<m; i++)
		{
			bool find = false;
			for (int j=0; j<n; j++)
				if (adj[i][j] == tmp[j])
				{
					find = true;
					break;	
				}	
			if (find == false)
			{
				ok = false;
				break;	
			}
		}
		if (ok == true)
		{
			int cnt = 0;
			for (int i=0; i<n; i++)
				if (tmp[i] == 1)
					cnt++;
			if (cnt < one)
			{
				one = cnt;
				for (int i=0; i<n; i++)
					ans[i] = tmp[i];	
			}
		}
	}	
	if (one == n+1)
	{
		printf(" IMPOSSIBLE\n");
		return;	
	}
	for (int i=0; i<n; i++)
		printf(" %d", ans[i]);
	printf("\n");
}

int main()
{
	//freopen("B-small.in", "r", stdin);
	//freopen("B-small.out", "w", stdout);
	
	int c;
	scanf("%d", &c);
	for (int kth=1; kth<=c; kth++)
	{
		scanf("%d%d", &n, &m);
		memset(adj, -1, sizeof(adj));
		for (int i=0; i<m; i++)
		{
			int t;
			scanf("%d", &t);
			while (t--)
			{
				int a, b;
				scanf("%d%d", &a, &b);
				a--;
				adj[i][a] = b;
			}	
		}
		printf("Case #%d:", kth);
		solve();	
	}	
	return 0;
}
