#include<stdio.h>
#define infinity 2147483647
int main()
{
	int l,n,min,j,i,k,r,t,x;
	int a[202];
	int f[202][202];
	freopen("2.in","r",stdin);
	freopen("2.out","w",stdout);
	scanf("%d",&t);
	for(x=1;x<=t;++x)
	{
		scanf("%d%d",&l,&n);
		for(i=1;i<=n;i++)
			scanf("%d",&a[i]);
		a[0]=0;a[n+1]=l+1;
		for(i=0;i<=n;i++)
			f[i][i+1]=0;
		for(r=2;r<=n+1;r++)
			for(i=0;i<=n-r+1;i++)
			{
				j=i+r;
				min=infinity;
				for(k=i+1;k<j;k++)
					if(f[i][k]+f[k][j]<min)min=f[i][k]+f[k][j];
				f[i][j]=min+a[j]-a[i]-2;
			}
		printf("Case #%d: %d\n",x,f[0][n+1]);
	}
	return 0;
}

