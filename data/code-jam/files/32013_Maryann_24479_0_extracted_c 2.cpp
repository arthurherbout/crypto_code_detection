#include<cstdio>
#include<cmath>
#define Pi acos(-1.0)
double ans,f,R,t,r,g,p;
double sqr(double x){return x<0?0:x*x;}
double calc(double x,double y,double l,double r)
{
    if(l<=0 || sqr(x)+sqr(y)>=sqr(r))
    return 0;
    if(sqr(x+l)+sqr(y)>=sqr(r))
    {
        if(sqr(x)+sqr(y+l)>=sqr(r))
        {
            double y1=sqrt(sqr(r)-sqr(x)),x1=sqrt(sqr(r)-sqr(y));
            return ((x1-x)*(y1-y)+r*r*fabs(atan2(y1,x)-atan2(y,x1))-fabs(x*y-x1*y1))/2;
        }
        double x1=sqrt(sqr(r)-sqr(y)),x2=sqrt(sqr(r)-sqr(y+l));
        return ((x1-x+x2-x)*l+r*r*fabs(atan2(y+l,x2)-atan2(y,x1))-fabs(x1*(y+l)-x2*y))/2;
    }
    double y2=sqrt(sqr(r)-sqr(x+l));
    if(sqr(x)+sqr(y+l)>=sqr(r))
    {
        double y1=sqrt(sqr(r)-sqr(x));
        return ((y1-y+y2-y)*l+r*r*fabs(atan2(y1,x)-atan2(y2,x+l))-fabs((x+l)*y1-x*y2))/2;
    }
    double y1=sqrt(sqr(r)-sqr(x+l)),x1=sqrt(sqr(r)-sqr(y+l));
    double t1=l*l-(x+l-x1)*(y+l-y1)/2;
    double t2=(r*r*fabs(atan2(y1,x+l)-atan2(y+l,x1))-fabs((x+l)*(y+l)-x1*y1))/2;
    return t1+t2;
}
int main()
{
    int cas;
    scanf("%d",&cas);
    for(int _=1; _<=cas; _++)
    {
        scanf("%lf%lf%lf%lf%lf",&f,&R,&t,&r,&g);
        t=R-t;
        ans=0;
        p=2*r+g;
        int n=(int)ceil(t/p);
        for(int i=0; i<n; i++)
        for(int j=0; j<n; j++)
        if(sqr((i+1)*p-r)+sqr((j+1)*p-r)<=t*t)
        ans+=sqr(g-2*f);
        else
        if(sqr(i*p+r)+sqr(j*p+r)<=t*t)
        /* calculate the area of the intersection of a square & a circle */
        ans+=calc(i*p+r+f,j*p+r+f,g-2*f,t-f);
        else
        break;
        printf("Case #%d: %.8lf\n",_,1-ans*4/(Pi*R*R));
    }
    return 0;
}
