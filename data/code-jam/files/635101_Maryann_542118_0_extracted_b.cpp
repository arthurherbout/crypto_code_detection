#include<stdio.h>
int n,k,q,a[50],v[50],ans;
bool b[50];
int main()
{
	int _,t,end,tim;
	scanf("%d",&_);
	for(t=1; t<=_; t++)
	{
		scanf("%d%d%d%d",&n,&k,&end,&tim);
		for(int i=0; i<n; i++)
			scanf("%d",&a[i]);
		for(int i=0; i<n; i++)
			scanf("%d",&v[i]);
		for(int i=0; i<n; i++)
			b[i]=(end-a[i]<=v[i]*tim);
		q=ans=0;
		for(int i=n-1; i>=0; i--)
			if(b[i])
			{
				q++;
				for(int j=i+1; j<n; j++)
					if(!b[j])
						ans++;
				if(q==k)break;
			}
		printf("Case #%d: ",t);
		if(q<k)puts("IMPOSSIBLE");else
			printf("%d\n",ans);
	}
	return 0;
}
