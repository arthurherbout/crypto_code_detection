#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>
#include <map>
#include <queue>
#include <iostream>
using namespace std;
int n,m,p,q;
#define maxn 1010
#define  LL  long long
map<pair<vector<int>,int>,int>dp;
int gg[maxn];
int dfs(vector<int>g,int k)
{
	if(dp.count(make_pair(g,k))) return dp[make_pair(g,k)];
	int ret=0;
	if(k==0) ret=max(ret,dfs(g,k^1));
	if(k==0)
	{
		for(int i=0;i<g.size();i++)
		{
			if(g[i]==0) continue;
			vector<int>tmp = g;
			if(g[i]-p<=0) tmp[i]=0,ret=max(ret,dfs(tmp,k^1)+gg[i]);
			else tmp[i]-=p,ret=max(ret,dfs(tmp,k^1));
		}
	}
	else
	{
		int i;
		for(i=0;i<g.size();i++)if(g[i]!=0)
			break;
		if(i!=g.size())
		{
			int xx=g[i];
			g[i]=max(0,g[i]-q);
			ret=max(ret,dfs(g,k^1));
			g[i]=xx;
		}
	}
	return dp[make_pair(g,k)]=ret;
}

int main()
{
	int ncase,i,j,tt=0;
	freopen("B-small-attempt1.in","r",stdin);
	freopen("B-small-attempt1.out","w",stdout);
	scanf("%d",&ncase);
	while(ncase--)
	{
		scanf("%d %d %d",&p,&q,&n);
		//printf("%d %d %d\n",p,q,n);
		vector<int>g; g.clear();
		for(i=0;i<n;i++)
		{
			scanf("%d",&j),g.push_back(j);
			scanf("%d",&gg[i]);
			//printf("%d %d\n",j,gg[i]);
		}
		dp.clear();
		printf("Case #%d: %d\n",++tt,max(dfs(g,0),dfs(g,1)));
	}
	return 0;

}

/*
22 22 4
195 36863
194 7256
194 112760
191 773419
*/