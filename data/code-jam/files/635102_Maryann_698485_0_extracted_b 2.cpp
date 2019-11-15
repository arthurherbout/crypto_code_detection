#include<stdio.h>
int a[1024],ans,n,m;
int main()
{
	int _,t;
	scanf("%d",&t);
	for(int _=1; _<=t; _++)
	{
		scanf("%d",&m);
		n=(1<<m);
		for(int i=0; i<n; i++)
			scanf("%d",&a[i]);
		ans=0;
		int qq;
		for(int i=m-1; i>=0; i--)
		{
			for(int j=0; j<(1<<i); j++)
			{
				scanf("%d",&qq);
				bool flag=true;
				for(int k=0; k<(1<<(m-i)); k++)
					if(a[(j<<(m-i))+k]==0)
					{
						flag=false;
						break;
					}
				if(flag)
					for(int k=0; k<(1<<(m-i)); k++)
						a[(j<<(m-i))+k]--;
				else
					ans+=qq;
			}
		}
		printf("Case #%d: %d\n",_,ans);
	}
	return 0;
}
