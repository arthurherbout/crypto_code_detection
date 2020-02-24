#include <stdio.h>

int main()
{
	int  aCase;
	long long n,a,b,c,d,x,y,m;
	long long point[100010][2];
	scanf("%d",&aCase);
	for(int k=1;k<=aCase;k++)
	{
		scanf("%lld%lld%lld%lld%lld%lld%lld%lld",&n,&a,&b,&c,&d,&x,&y,&m);
		point[0][0]=x;
		point[0][1]=y;
		for(int i=1;i<=n-1;i++)
		{
			point[i][0]=(point[i-1][0]*a+b)%m;
			point[i][1]=(point[i-1][1]*c+d)%m;
		}
		long long count=0;
		for(int i=0;i<n;i++)
		{
			for(int j=i+1;j<n;j++)
			{
				if(i==j)
					continue;
				for(int ii=j+1;ii<n;ii++)
				{
					if(ii==i||ii==j)
						continue;
					if((point[i][0]+point[j][0]+point[ii][0])%3==0&&(point[i][1]+point[j][1]+point[ii][1])%3==0)
						count++;
				}
			}
		}
		printf("Case #%d: %lld\n",k,count);
	}
	return 0;
}
