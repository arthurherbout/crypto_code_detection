#include<stdio.h>
#include<math.h>
#include<algorithm>
using namespace std;
int n;
double x[30],y[30],r[30],ans;
double sqr(double x){return x*x;}
int main()
{
	int _,t;
	scanf("%d",&_);
	for(t=1; t<=_; t++)
	{
		scanf("%d",&n);
		for(int i=0; i<n; i++)
			scanf("%lf%lf%lf",&x[i],&y[i],&r[i]);
		if(n==1)
			ans=r[0];
		else
		if(n==2)
			ans=max(r[0],r[1]);
		else
		{
		ans=1e9;
		for(int i=0; i<n; i++)
			for(int j=i+1; j<n; j++)
				ans=min(ans,max(r[0]+r[1]+r[2]-r[i]-r[j],(sqrt(sqr(x[i]-x[j])+sqr(y[i]-y[j]))+r[i]+r[j])/2));
		}
		printf("Case #%d: %lf\n",t,ans);
	}
	return 0;
}
