#include<stdio.h>
#include<string.h>
#define mod 10007
int n,m,r,d[103][103];
int dp(int x,int y)
{
	if(d[x][y]==-1)
	if(x==1 && y==1)
	d[x][y]=1;
	else
	{
		d[x][y]=0;
		if(x>1 && y>2)d[x][y]=dp(x-1,y-2);
		if(x>2 && y>1)
		{
			d[x][y]+=dp(x-2,y-1);
			if(d[x][y]>=mod)d[x][y]-=mod;
		}
	}
	return d[x][y];
}
int main()
{
	int _,x,y;
	scanf("%d",&_);
	for(int t=1; t<=_; t++)
	{
		scanf("%d%d%d",&n,&m,&r);
		memset(d,-1,sizeof(d));
		for(int i=0; i<r; i++)
		{
			scanf("%d%d",&x,&y);
			d[x][y]=0;
		}
		printf("Case #%d: %d\n",t,dp(n,m));
	}		
	return 0;
}
