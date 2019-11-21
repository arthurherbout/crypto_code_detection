#include <iostream>

using namespace std;

string s[10];
int n, m;
int ans, at;
int ctree[100][30];
int cl;

void dfs(int x, int a[10])
{
	if (x == m)
	{
		bool vis[10];
		memset(vis, 0, sizeof(vis));
		for (int i = 0 ; i < m ; i++) vis[a[i]] = true;
		int tot = 0;
		for (int i = 1 ; i <= n ; i++)
			if (vis[i])
				tot ++;
		if (tot < n) return;
		memset(ctree, 0, sizeof(ctree));
		cl = n;
		int tnode = 0;
		for (int i = 0; i < m; i++)
		{
			tnode = a[i];
			for (int j = 0 ; j < s[i].length() ; j++)
			{
				if (ctree[tnode][s[i][j] - 'A'] == 0)
				{
					cl ++;
					ctree[tnode][s[i][j] - 'A'] = cl;
					tnode = cl;
				}
				else {
					tnode = ctree[tnode][s[i][j] - 'A'];
				}
			}
		}
		if (cl == ans) at ++;
		else
		if (cl > ans)
		{
			ans = cl;
			at = 1;
		}
		//for (int i = 0 ; i < m ; i++) cout << a[i] << " "; cout << endl;
		return;
	}

	for (int i = 1 ; i <= n ; i++)
	{
		a[x] = i;
		dfs(x + 1, a);
	}
}

int main()
{
	freopen("D-small-attempt0.in", "r", stdin);
	freopen("D-small-attempt0.out", "w", stdout);
	int T;
	cin >> T;
	for (int tt = 1 ; tt <= T ; tt++)
	{
		cin >> m >> n;
		for (int i = 0 ; i < m ; i++)
			cin >> s[i];
		ans = 0;
		at = 0;
		int a[10];
		dfs(0, a);
		cout << "Case #" << tt << ": " << ans << " " << at << endl;
	}
}