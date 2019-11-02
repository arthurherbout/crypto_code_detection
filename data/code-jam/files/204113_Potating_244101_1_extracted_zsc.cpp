#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define N 201
int map[N][N], use[N], link[N], n, m, k, stack[N][N];
int can(int t)
{
	int i;
	for(i=1;i<=m;i++)
	{
		if(!use[i]&&map[t][i])
		{
			use[i]=1;
			if(link[i]==-1||can(link[i]))
			{
				link[i]=t;
				return 1;
			}
		}
	}
	return 0;
}

int MaxMatch()
{
	int i, num;
	num=0;
	memset(link,0xff,sizeof(link));
	for(i=1;i<=n;i++)
	{
		memset(use,0,sizeof(use));
		if(can(i)) num++;
	}
	return num;
}

void solve()
{
	int i, t, p, ans, j, l, flag;
	scanf("%d%d",&n,&k);
	m=n;
	memset(map,0,sizeof(map));
	for(i=1;i<=n;i++)
		for(j=1;j<=k;j++)
			scanf("%d",&stack[i][j]);
	for(i=1;i<=n;i++)
		for(j=1;j<=n;j++)
		{
			if(i==j) continue;
			flag=1;
			for(l=1;l<=k;l++)
			{
				if(stack[i][l]<=stack[j][l])
				{
					flag=0;
					break;
				}
			}
			if(flag) map[i][j]=1;
		}
	ans=MaxMatch();
	printf("%d\n",n-ans);
}
int main()
{
	freopen("C-large.in","r",stdin);
	freopen("cut_large.out","w",stdout);
	int T, i;
	scanf("%d",&T);
	for(i=1;i<=T;i++)
	{
		printf("Case #%d: ",i);
		solve();
	}
	return 0;
}

