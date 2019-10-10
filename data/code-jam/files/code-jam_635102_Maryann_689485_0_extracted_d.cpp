#include<stdio.h>
#include<math.h>
int n,m;
double x[9],y[9];
double X,Y;
double pi=acos(-1.0);
double sqr(double x){return x*x;}
double calc(double ax,double ay,double x,double y,double bx,double by)
{
	double ang=atan2(y-ay,x-ax)-atan2(by-ay,bx-ax);
	if(ang<0)ang+=2*pi;else
		if(ang>2*pi)ang-=2*pi;
	if(ang>=pi)ang=2*pi-ang;
	ang*=2;
	double l2=sqr(x-ax)+sqr(y-ay);
	double ans=ang/2*l2;
	if(ang>=pi)
		ans+=sin(2*pi-ang)/2*l2;
	else
		ans-=sin(ang)/2*l2;
	return ans;
}
int main()
{
	int _,t;
	scanf("%d",&t);
	for(_=1; _<=t; _++)
	{
		scanf("%d%d",&n,&m);
		for(int i=0; i<n; i++)
			scanf("%lf%lf",&x[i],&y[i]);
		printf("Case #%d: ",_);
		while(m--)
		{
			scanf("%lf%lf",&X,&Y);
			printf(" %.10lf",fabs(calc(x[0],y[0],X,Y,x[1],y[1])+
					calc(x[1],y[1],X,Y,x[0],y[0])));
		}
		puts("");
	}
	return 0;
}
