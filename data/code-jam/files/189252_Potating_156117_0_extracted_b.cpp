#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#define inf 1e-10
#define N 510
int x[N], y[N], z[N];
int vx[N], vy[N], vz[N];
double sumx, sumy, sumz, sumvx, sumvy, sumvz;
int n;
void solve()
{
	int i, j;
	double tmp;
	double a, b, c, d, e, f, aa, bb, cc;
	for(i=0;i<n;i++)
		scanf("%d%d%d%d%d%d",x+i,y+i,z+i,vx+i,vy+i,vz+i);
	sumx=0; sumy=0; sumz=0;
	sumvx=0; sumvy=0; sumvz=0;
	double ansd=0, anst=0;
	for(i=0;i<n;i++)
	{
		sumx+=x[i];
		sumy+=y[i];
		sumz+=z[i];
		sumvx+=vx[i];
		sumvy+=vy[i];
		sumvz+=vz[i];
	}
	a=sumx; b=sumvx;
	c=sumy; d=sumvy;
	e=sumz; f=sumvz;
	aa=(b*b+d*d+f*f);
	bb=2*(a*b+c*d+e*f);
	cc=a*a+c*c+e*e;
	if(aa==0)
	{
		ansd=sqrt((double)(sumx*sumx+sumy*sumy+sumz*sumz));
		ansd/=n;
		anst=0;
		printf("%.8lf %.8lf\n",ansd,anst);
	}
	else
	{
		tmp=-(1.0)*bb/(2*aa);
		if(tmp<inf)
		{
			ansd=sqrt((double)(sumx*sumx+sumy*sumy+sumz*sumz));
			ansd/=n;
			anst=0;
			printf("%.8lf %.8lf\n",ansd,anst);
		}
		else
		{
			anst=tmp;
			for(i=0;i<n;i++)
			{
				sumx+=vx[i]*anst;
				sumy+=vy[i]*anst;
				sumz+=vz[i]*anst;
			}
			ansd=sqrt((double)(sumx*sumx+sumy*sumy+sumz*sumz));
			ansd/=n;
			printf("%.8lf %.8lf\n",ansd,anst);
		}
	}
}
int main()
{
	int T, i;
	freopen("B-small-attempt3.in","r",stdin);
	freopen("Bbbb333333.out","w",stdout);
	scanf("%d",&T);
	for(i=1;i<=T;i++)
	{
		printf("Case #%d: ",i);
		scanf("%d",&n);
		solve();
	}
	return 0;
}

