#include<stdio.h>
#include<math.h>

int main()
{
long double cx=0,cy=0,cz=0;
long  double cvx=0,cvy=0,cvz=0;
long double ta=0;

int x=0,y=0,z=0;
int vx=0,vy=0,vz=0;

int t,n;
int i,j,k;
scanf("%d",&t);
for(i=0;i<t;i++)
{
scanf("%d",&n);
cx=0,cy=0,cz=0;
cvx=0,cvy=0,cvz=0;
ta=0;
for(j=0;j<n;j++)
{
scanf("%d%d%d%d%d%d",&x,&y,&z,&vx,&vy,&vz);
cx+=x;
cy+=y;
cz+=z;
cvx+=vx;
cvy+=vy;
cvz+=vz;


}

long double ans=cz*cz+cy*cy+cx*cx;
if(cvx*cvx+cvz*cvz+cvy*cvy<=0.000001)
{long double a=0;
printf("Case #%d: %.12Lf %.12Lf\n",i+1,sqrtl(ans)/n,a);
continue;
}
ta=-(cx*cvx+cvy*cy+cvz*cz)/(cvx*cvx+cvz*cvz+cvy*cvy);

long double dx=cvx*ta;
long double dy=cvy*ta;
long double dz=cvz*ta;
cx+=dx;
cy+=dy;
cz+=dz;



ans=cz*cz+cy*cy+cx*cx;


printf("Case #%d: %.12Lf %.12Lf\n",i+1,sqrtl(ans)/n,ta);

}


return 0;}
