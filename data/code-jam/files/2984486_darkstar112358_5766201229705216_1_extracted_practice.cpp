#include <list>
#include <map>
#include <set>
#include <queue>
#include <deque>
#include <stack>
#include <bitset>
#include <algorithm>
#include <functional>
#include <numeric>
#include <utility>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <fstream>

using namespace std;

#define inf 1000000000
#define maxN 1005

vector < vector < int > > adj;

int subTreeCount[maxN];
int vis[maxN];

int calc1(int pos)
{
	vis[pos] = 1;
	int ans = 0;
	for(int i=0;i<adj[pos].size();i++)
		if( vis[adj[pos][i]] == 0 )
			ans += 1+calc1(adj[pos][i]);
	return subTreeCount[pos] = ans;
}

int solve(int pos)
{

	//you can remove all the nodes in its subtree
	vis[pos] = 1;
	int ans = subTreeCount[pos];

	vector<int> children;
	for(int i=0;i<adj[pos].size();i++)
	{
		if( vis[adj[pos][i]] == 1 )
			continue;
		children.push_back(subTreeCount[adj[pos][i]]-solve(adj[pos][i]));
	}

	if( children.size() < 2 )
		return ans;

	sort(children.begin(), children.end());
	int X = children.size();

	ans = min(ans,subTreeCount[pos]-2-children[X-1]-children[X-2]);

	return ans;

}

int main()
{

	freopen("input.txt","r",stdin);
	int T;
	scanf("%d",&T);
	for(int t1=1;t1<=T;t1++)
	{

		printf("Case #%d: ",t1);

		adj.clear();
		int N;
		scanf("%d",&N);

		vector<int> temp1;
		for(int i=0;i<N;i++)
			adj.push_back(temp1);

		for(int i=0;i<N-1;i++)
		{
			int u, v;
			scanf("%d%d",&u,&v);
			u--, v--;
			adj[u].push_back(v);
			adj[v].push_back(u);
		}

		int ans = N-1;

		for(int root=0;root<N;root++)
		{

			for(int i=0;i<N;i++)
				vis[i] = 0;

			calc1(root);

			for(int i=0;i<N;i++)
				vis[i] = 0;

			int y = solve(root);
			ans = min(ans,y);

		}

		printf("%d\n",ans);

	}

	return 0;

}
