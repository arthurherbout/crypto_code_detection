#include<iostream>
#include<vector>
using namespace std;
vector<long long> v;
long long dp[1005];
int main()
{
    freopen("C-small-attempt0.in","r",stdin);
    freopen("C-small-attempt0.out","w",stdout);
	long long n,m,X,Y,Z,a[1005];
	int cas,test=1,i,j;
	scanf("%d",&cas);
	while(cas--)
	{
		v.clear();
		cin>>n>>m>>X>>Y>>Z;
		for(i=0;i<m;i++)
		{
			cin>>a[i];
		}
		for(i=0;i<n;i++)
		{
			v.push_back(a[i%m]);
			a[i%m] = (X * a[i%m] + Y * (i + 1))%Z;
		}
		for(i=0;i<v.size();i++)
		dp[i]=1;
		for(i=0;i<v.size();i++)
		{
			for(j=i+1;j<v.size();j++)
			{
				if(v[i]<v[j]) dp[j]=(dp[i]+dp[j])%1000000007;
			}
		}
		int sum=0;
		for(i=0;i<v.size();i++)
		{
			sum=(sum+dp[i])%1000000007;
		}
		cout<<"Case #"<<test++<<": "<<sum<<endl;
	}
	return 0;
}
