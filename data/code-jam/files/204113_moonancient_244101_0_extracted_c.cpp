#include <iostream>
#include <algorithm>
using namespace std;
#define MAXN 110
#define MAXM 30
int n,m;
int price[MAXN][MAXM];

int ans;
bool adj[MAXN][MAXN];
int lmatch[MAXN];
int rmatch[MAXN];

bool flag[MAXN];
bool dfs(int u)
{
	flag[u]=false;
	for (int v=0;v<n;v++)
		if (adj[u][v] && lmatch[u]!=v)
		{
			int w=rmatch[v];
			if (w==-1 || (flag[w] && dfs(w)))
			{
				lmatch[u]=v;
				rmatch[v]=u;
				return true;
			}
		}
	return false;
}

int t;
int main()
{
	cin>>t;
	for (int caseI=1;caseI<=t;caseI++)
	{
		cin>>n>>m;
		for (int i=0;i<n;i++)
			for (int j=0;j<m;j++)
				cin>>price[i][j];
		memset(adj,false,sizeof(adj));
		for (int i=0;i<n;i++)
			for (int j=0;j<n;j++)
			{
				bool flag=true;
				for (int k=0;k<m && flag;k++)
					if (price[i][k]<=price[j][k]) flag=false;
				if (flag) adj[i][j]=true;
			}
		memset(lmatch,-1,sizeof(lmatch));
		memset(rmatch,-1,sizeof(rmatch));
		ans=n;
		for (int i=0;i<n;i++)
			if (lmatch[i]==-1)
			{
				memset(flag,true,sizeof(flag));
				if (dfs(i)) ans--;
			}
		cout<<"Case #"<<caseI<<": "<<ans<<endl;
	}
	return 0;
}

