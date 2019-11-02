#include<stdio.h>
#include<math.h>
#define esp 1e-9
double x[600],y[600],z[600],vx[600],vy[600],vz[600];
int main(){
	freopen("15.in","r",stdin);
	freopen("15.out","w",stdout);
	int t,i,k,n;
	double xx,yy,zz,vxx,vyy,vzz,a,b,c;
	scanf("%d",&t);
	for(k=1;k<=t;++k){
		scanf("%d",&n);
		xx=yy=zz=vxx=vyy=vzz=0.0;
		for(i=0;i<n;++i){
			scanf("%lf%lf%lf%lf%lf%lf",x+i,y+i,z+i,vx+i,vy+i,vz+i);
			xx+=x[i];yy+=y[i];zz+=z[i];vxx+=vx[i];vyy+=vy[i];vzz+=vz[i];
		}
		a=vxx*vxx+vyy*vyy+vzz*vzz;
		b=2.0*(xx*vxx+yy*vyy+zz*vzz);
		c=xx*xx+yy*yy+zz*zz;
		double t1;
		if(fabs(a-0.0)>esp)t1=-0.5*b/a;
		else t1=0.00000;
		if(t1>0.0){
			double len=a*t1*t1+b*t1+c;
			double dis;
			if(fabs(len)<esp)dis=0.0000;
			else{
				dis=sqrt(len);
				if(fabs(len)>esp)dis/=(double)n;
			}
			printf("Case #%d: %.8lf %.8lf\n",k,dis,t1);
		}
		else{
			double dis=sqrt(c);
			if(fabs(dis)>esp)dis/=(double)n;
			t1=0.000000;
			printf("Case #%d: %.8lf %.8lf\n",k,dis,t1);
		}
	}
	return 0;
}
