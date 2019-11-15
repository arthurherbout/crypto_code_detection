#include <stdio.h>
#include <algorithm>
using namespace std;

struct Node
{
	int ListA[4001];
	int ListB[4001];
	int num ;
}node[1001];
int n,m;
int temp[1001] ;
int ans[1001] ;
int Num ;
bool judge()
{
	int i,j;
	for(i = 0 ; i < m ; i ++)
	{
		for(j = 0 ; j < node[i].num ; j ++)
		{
			if(temp[node[i].ListA[j]] == node[i].ListB[j])
				break ;
		}
		if(j == node[i].num)
			return 0 ;
	}
	if(i == m)
		return 1 ;
	return 0 ;
}
void dfs(int now)
{
	int i;
	if(now == n)
	{
		if(judge())
		{
			int Count = 0 ;
			for(i = 0 ; i < n ; i ++)
			{
				if(temp[i] == 1)
					Count ++ ;
			}
			if(Num == -1 || Count < Num)
			{
				for(i = 0 ; i < n ; i ++)
				{
					ans[i] = temp[i] ;
				}
				Num = Count ;
			}
		}
		return ;
	}
	for(i = 0 ; i < 2 ; i ++)
	{
		temp[now] = i ;
		dfs(now+1) ;
	}
}
void solve()
{
	int i;
	Num = -1 ;
	dfs(0);
	if(Num == -1)
	{
		printf(" IMPOSSIBLE\n");
		return ;
	}
	for(i = 0 ; i < n ; i ++)
	{
		printf(" %d",ans[i]) ;
	}
	printf("\n");
}
int main()
{
	int t,i,j,k,a,b,T;
	freopen("B-small-attempt0.in","r",stdin);
	freopen("B-small.out","w",stdout);
	while(1 == scanf("%d",&t))
	{
		for(i = 1 ; i <= t ; i ++)
		{
			scanf("%d",&n);
			scanf("%d",&m);
			for(k = 0 ; k < m ; k ++)
			{
				scanf("%d",&T);
				for(j = 0 ; j < T ; j ++)
				{
					scanf("%d %d",&a,&b);
					a -- ;
					node[k].ListA[j] = a ;
					node[k].ListB[j] = b ;
				}
				node[k].num = T ;
			}
			printf("Case #%d:",i);
			solve() ;
		}
	}
	return 0;
}