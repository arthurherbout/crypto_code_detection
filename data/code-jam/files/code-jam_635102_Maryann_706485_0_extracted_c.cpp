#include<stdio.h>
#include<string.h>
int a[110][110],b[110][110];
int main()
{
	int _,t;
	int ax,ay,bx,by,r;
	scanf("%d",&t);
	for(int _=1; _<=t; _++)
	{
		scanf("%d",&r);
		memset(a,0,sizeof(a));
		while(r--)
		{
			scanf("%d%d%d%d",&ax,&ay,&bx,&by);
			for(int i=ax; i<=bx; i++)
				for(int j=ay; j<=by; j++)
					a[i][j]=1;
		}
		int ans=0;
		for(;;)
		{
			bool flag=false;
			for(int i=1; i<=100; i++)
				for(int j=1; j<=100; j++)
				{
					b[i][j]=a[i][j];
					if(a[i][j])
					{
						flag=true;
						if(!a[i-1][j] && !a[i][j-1])
							b[i][j]=0;
					}
					else
					if(a[i-1][j] && a[i][j-1])
						b[i][j]=1;
				}
			if(!flag)break;
			memcpy(a,b,sizeof(a));
			ans++;
		}
		printf("Case #%d: %d\n",_,ans);
	}
	return 0;
}
