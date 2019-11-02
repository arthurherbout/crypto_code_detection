#include<stdio.h>
#include<queue>
#include<utility>
#include<algorithm>
using namespace std;
int n;
long long int a[100],l,g;
long long int gcd(long long int x,long long int y){return y?gcd(y,x%y):x;}
int dp[100010];
priority_queue<pair<int,long long int> > q;
int main()
{
	int t,_;
	scanf("%d",&_); 
	for(t=1; t<=_; t++)
	{
		scanf("%I64d%d",&l,&n);
		g=0;
		for(int i=0; i<n; i++)
		{
			scanf("%I64d",&a[i]);
			g=gcd(g,a[i]);
		}
		sort(a,a+n);
		printf("Case #%d: ",t);
		if(l%g)
		{
			puts("IMPOSSIBLE");
			continue;
		}
		memset(dp,-1,sizeof(dp));
		dp[0]=0;
		q.push(make_pair(0,0));
		n--;
		int u=a[n];
		while(!q.empty())
		{
			int c=q.top().first;
			long long int l1=-q.top().second;
			q.pop();
			if(dp[c]!=l1)continue;
			for(int i=0; i<n; i++)
			{
				long long int l2=c+a[i],cost=dp[c]+1;
				if(l2>=u)l2-=u,cost--;
				if(dp[l2]==-1 || dp[l2]>cost)
				{
					dp[l2]=cost;
					q.push(make_pair((int)(l2),-dp[l2]));
				}
			}
		}
		long long int w=l%a[n];
		long long int ans=dp[w]+(l-w)/a[n];
		printf("%I64d\n",ans);
		fprintf(stderr,"%d\n",t);
	}
	return 0;
}
