#include<stdio.h>
#include<algorithm>
#define r 40
using namespace std;
int n,m;
double c[r+1][r+1],dp[r+1],ans[r+1][r+1];
int main()
{
	int i,j,_;
	c[0][0]=1;
	for(i=1; i<=r; i++)
		for(j=0; j<=i; j++)
			if(j==0 || j==i)
				c[i][j]=1;
			else
				c[i][j]=c[i-1][j]+c[i-1][j-1];
	for(n=1; n<=r; n++)
		for(m=1; m<=n; m++)
		{
			dp[n]=0;
			for(i=n-1; i>=0; i--)
			{
				dp[i]=1;
				for(j=max(1,m-i); j<=m && j+i<=n; j++)
					dp[i]+=(c[i][m-j]*c[n-i][j]/c[n][m])*dp[i+j];
				if(i>=m)
					dp[i]/=(1-c[i][m]/c[n][m]);
			}
			ans[n][m]=dp[0];
		}
	scanf("%d",&_);
	for(i=1; i<=_; i++)
	{
		scanf("%d%d",&n,&m);
		printf("Case #%d: %.7lf\n",i,ans[n][m]);
	}
	return 0;
}
