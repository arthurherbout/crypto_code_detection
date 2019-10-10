#include <iostream>
#include <algorithm>
#include <cmath>
using namespace std;
#define MAXN 41

double eps=1e-9;

int m,n;
long long c[MAXN][MAXN];
double p[MAXN][MAXN];
double q[MAXN][MAXN];
double ans;

int t;
int main()
{
	c[0][0]=1;
	for (int i=1;i<MAXN;i++)
	{
		c[i][0]=c[i][i]=1;
		for (int j=1;j<i;j++)
			c[i][j]=c[i-1][j]+c[i-1][j-1];
	}
	cin>>t;
	cout.setf(ios::fixed);
	cout.precision(7);
	for (int caseI=1;caseI<=t;caseI++)
	{
		cin>>m>>n;
		memset(p,0,sizeof(p));
		for (int i=0;i<=m;i++)
			for (int j=0;j<=n;j++)
				if (j<=i && i+n-j<=m)
				{
					p[i][i+n-j]+=c[i][j]*1.0*c[m-i][n-j]/(1.0*c[m][n]);
				}
		p[m][m]=0;
		memset(q,0,sizeof(q));
		for (int i=0;i<m;i++)
			q[i][i]=1;
		ans=0;
		for (int now=1;now<=100 || q[0][m]>eps;now++)
		{
			double temp[MAXN][MAXN];
			memset(temp,0,sizeof(temp));
			for (int i=0;i<=m;i++)
				for (int j=i;j<=m;j++)
					for (int k=j;k<=m;k++)
						temp[i][k]+=q[i][j]*p[j][k];
			ans+=now*temp[0][m];
			memcpy(q,temp,sizeof(q));
			q[m][m]=0;
		}
		cout<<"Case #"<<caseI<<": "<<ans<<endl;
	}
	return 0;
}

