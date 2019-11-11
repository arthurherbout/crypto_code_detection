#include <cstdio>
#include <iostream>
#include <algorithm>
using namespace std;

bool vac[200];
int T,P,Q,cas=1;
int num[10],res;

void dfs(int cur,int sum)
{
	if(cur==Q)
	{
		if(sum<res) res=sum;
		return ;
	}
	vac[num[cur]]=true;
	int nx=num[cur]-1,cnt=0;
	while(nx>=1&&!vac[nx--])
		cnt++;
	nx=num[cur]+1;
	while(nx<=P&&!vac[nx++])
		cnt++;
	dfs(cur+1,sum+cnt);
}

int main()
{
	freopen("d://C-small-attempt0.in","r",stdin);
	freopen("d://1.txt","w",stdout);
	scanf("%d",&T);
	while(T--)
	{
		scanf("%d%d",&P,&Q);
		for(int i=0;i<Q;i++)
			scanf("%d",&num[i]);
		res=1000000000;
		do
		{
			memset(vac,false,sizeof(vac));
			dfs(0,0);
		}while(next_permutation(num,num+Q));
		printf("Case #%d: %d\n",cas++,res);
	}
	return 0;
}