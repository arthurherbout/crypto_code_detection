#include<stdio.h>
#include<stdlib.h>
#include<algorithm>
using namespace std;
int n;
int A[440][440],*A1[440],**a;
int main()
{
	int _,t;
	scanf("%d",&_);
	for(int i=0; i<440; i++)
		A1[i]=&A[i][220];
	a=&A1[220];
	for(t=1; t<=_; t++)
	{
		scanf("%d",&n);
		memset(A,-1,sizeof(A));
		for(int i=1; i<2*n; i++)
		{
			int L=n-labs(n-i);
			for(int j=n-L+1; j<=n+L-1; j+=2)
				scanf("%d",&a[i][j]);
		}
		int ans=4*n;
		for(int i=1; i<2*n; i++)
		{
			int L=n-labs(n-i);
			for(int j=0; j<=2*n; j++)
			{
				bool flag=true;
				for(int i1=1; i1<2*n && flag; i1++)
				{
					int L1=n-labs(n-i1);
					for(int j1=n-L1+1; j1<=n+L1-1 && flag; j1+=2)
					{
						int si1,sj1;
						si1=i*2-i1,sj1=j*2-j1;
						if(a[si1][sj1]!=-1 && a[si1][sj1]!=a[i1][j1])
							flag=false;
						si1=i1,sj1=j*2-j1;
						if(a[si1][sj1]!=-1 && a[si1][sj1]!=a[i1][j1])
							flag=false;
						si1=i*2-i1,sj1=j1;
						if(a[si1][sj1]!=-1 && a[si1][sj1]!=a[i1][j1])
							flag=false;
					}
				}
				if(flag)
					ans=min(ans,n+(int)(labs(n-i)+labs(n-j)));
			}
		}
		printf("Case #%d: %d\n",t,ans*ans-n*n);
	}
	return 0;
}