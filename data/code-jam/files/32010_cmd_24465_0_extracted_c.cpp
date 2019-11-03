#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <queue>
#include <map>
#include <stack>
#include <functional>
using namespace std;


typedef long long LL;
#define mp make_pair
#define pb push_back
#define X first
#define Y second
const int mod = 1000000009;

int n, m, k;
vector< vector<int> > G;
int qq[777][3];

LL dfs(int u, int pred, int k, int p1 = 0, int p2 = 0, int p3 = 0)
{
	LL res = 1;
	if(k < 0) return 0;
	int sum = 0;
	for(int i = 0; i < G[u].size(); ++i) if(G[u][i] != pred)
	{
		int v = G[u][i];
		//int z = qq[u][1] - qq[v][0] + qq[u][2] - qq[v][1] + 1;

		res *= dfs(v, u, k - sum + p1 - 1, p2, p3, sum + 1) * LL(k - sum + p1);
		if(k - sum + p1 < 0) res = 0;
		res %= mod;
		sum +=  1;
	}
	if(res < 0) res =0;
	return res;
}
bool passed[777];
int main()
{
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
	int t; scanf("%d", &t);
	for(int z = 1; z <= t; ++z)
	{

		G.clear();
		
		scanf("%d%d", &n, &k);
		G.resize (n);
		for(int i = 0; i < n - 1; ++i)
		{
			int u, v; 
			scanf("%d%d", &u, &v);
			u--; v--;
			G[u].push_back(v);
			G[v].push_back(u);
		}
		memset(qq, 0, sizeof qq);
		for(int i = 0; i < n; ++i)
		{
			int cnt = 0;
			for(int j = 0; j < G[i].size(); ++j)
				cnt += G[G[i][j]].size() - 1;
			qq[i][1] = G[i].size();
			qq[i][2] = cnt;
			qq[i][0] = 1;
		}
		memset(passed, 0, sizeof passed);
		LL res = dfs(0, -1, k);
		cout << "Case #" << z << ": " << res << endl;
		
		
	}
	return 0;
}