#include<stdio.h>
#define mod 100003
long long int dp[510][510],ans[510];
long long int c[510][510];
void init()
{
	for(int i=0; i<=500; i++)
		for(int j=0; j<=i; j++)
			if(j==0 || j==i)
				c[i][j]=1;
			else
				c[i][j]=(c[i-1][j]+c[i-1][j-1])%mod;
	for(int i=2; i<=500; i++)
	{
		dp[i][1]=ans[i]=1;
		for(int j=2; j<i; j++)//the rank of i in that set
		{
			for(int k=1; k<j; k++)//the rank of j in that set
				dp[i][j]+=dp[j][k]*c[i-j-1][j-k-1]%mod;
			dp[i][j]%=mod;
			ans[i]+=dp[i][j];
		}
		ans[i]%=mod;
	}
}
int main()
{
	int _,t,n;
	init();
	scanf("%d",&_);
	for(t=1; t<=_; t++)
	{
		scanf("%d",&n);
		printf("Case #%d: %I64d\n",t,ans[n]);
	}
	return 0;
}
