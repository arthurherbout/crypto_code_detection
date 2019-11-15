#include<stdio.h>
#include<string.h>
int n,m,a[110][100],c[110][110],lk[110];
int ans;
bool b[110];
bool fd(int x)
{
	for(int i=0; i<n; i++)
		if(!b[i] && c[x][i])
		{
			b[i]=1;
			if(lk[i]==-1 || fd(lk[i]))
			{
				lk[i]=x;
				return true;
			}
		}
	return false;
}
int main()
{
	int _,t;
	scanf("%d",&_);
	for(t=1; t<=_; t++)
	{
		scanf("%d%d",&n,&m);
		for(int i=0; i<n; i++)
			for(int j=0; j<m; j++)
				scanf("%d",&a[i][j]);
		for(int i=0; i<n; i++)
			for(int j=0; j<n; j++)
				if(j!=i)
				{
					bool flag=true;
					for(int k=0; k<m && flag; k++)
						if(a[i][k]>=a[j][k])
							flag=false;
					c[i][j]=flag;
				}
		memset(lk,-1,sizeof(lk));
		ans=n;
		for(int i=0; i<n; i++)
		{
			memset(b,0,sizeof(b));
			if(fd(i))
				ans--;
		}
		printf("Case #%d: %d\n",t,ans);
	}
	return 0;
}
