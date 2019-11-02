#include<stdio.h>
#define ran 10000
long long int d,a[15];
int n;
int fac[ran+10],pr[ran],sum;
int main()
{
	int _,t;
	scanf("%d",&_);
	for(int i=2; i<=ran; i++)
	{
		if(fac[i]==0)
		{
			fac[i]=i;
			pr[sum++]=i;
		}
		for(int j=0; j<sum; j++)
		{
			if(i*pr[j]>=ran)break;
			fac[i*pr[j]]=pr[j];
			if(i%pr[j]==0)break;
		}
	}
	long long int ax;
	for(t=1; t<=_; t++)
	{
		scanf("%I64d%d",&d,&n);
		ax=-1;
		for(int i=0; i<n; i++)
		{
			scanf("%I64d",&a[i]);
			if(a[i]>ax)ax=a[i];
		}
		printf("Case #%d: ",t);
		if(n==1)
		{
			puts("I don't know.");
			continue;
		}
		long long int tt=1,p;
		while(d--)tt*=10;
		d=tt;
		bool fflag=true;
		long long int ans=-1;
		for(int pind=0; pind<sum && fflag; pind++)
			if((p=pr[pind])<tt)
			{
				if(p<=ax)continue;
				for(long long int A=0; A<p; A++)
				{
					long long int B=-1,bb;
					bool flag=true;
					for(int i=0; i<n-1; i++)
					{
						bb=(a[i+1]-a[i]*A%p+p)%p;
						if(B==-1)B=bb;else
							if(B!=bb)
							{
								flag=false;
								break;
							}
					}
					if(flag)
					{
						B=(A*a[n-1]+B)%p;
						if(ans==-1)ans=B;else
							if(ans!=B)
							{
								fflag=false;
								break;
							}
					}
				}
			}
			else
				break;
		if(fflag)
			printf("%I64d\n",ans);
		else
			puts("I don't know.");
		fprintf(stderr,"%d\n",t);
	}
	return 0;
}
