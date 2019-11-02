#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <cmath>

using namespace std;

double EPS=1e-10;

double cal(double a,double b,double h,double A)
{
   double aa=b/h-a/h,bb=a+a,cc=-A-A,delta=bb*bb-4.0*aa*cc;
   if (fabs(aa)<EPS) return -cc/bb;
   return 0.5/aa*(-bb+sqrt(max(delta,0.0)));
}

int lx[105],ly[105],ux[105],uy[105];
double ll[1005],uu[1005],a[1005],aa,tt;

int main(void)
{
    freopen("A-large.in","r",stdin);
    freopen("A-large.out","w",stdout);
    int T,i,j,k,w,l,u,g,U;
    scanf("%d",&T);
    for(U=1;U<=T;++U)
    {
        scanf("%d%d%d%d",&w,&l,&u,&g);
        for(i=1;i<=l;++i)
            scanf("%d%d",lx+i,ly+i);
        for(i=1;i<=u;++i)
            scanf("%d%d",ux+i,uy+i);
        aa=0.0;
        for(k=0;k<=w;++k)
        {
            for(i=1;i<l;++i)
                if (lx[i]<=k&&lx[i+1]>=k)
                {
                    ll[k]=double(ly[i]*(lx[i+1]-k)+ly[i+1]*(k-lx[i]))/(lx[i+1]-lx[i]);
                    break;
                }
            for(i=1;i<u;++i)
                if (ux[i]<=k&&ux[i+1]>=k)
                {
                    uu[k]=double(uy[i]*(ux[i+1]-k)+uy[i+1]*(k-ux[i]))/(ux[i+1]-ux[i]);
                    break;
                }
            if (k>0)
            {
                a[k]=0.5*(uu[k]-ll[k]+uu[k-1]-ll[k-1]);
                aa+=a[k];
            }
        }
        printf("Case #%d:\n",U);
        for(k=1;k<g;++k)
        {
            tt=aa*k/g;
            for(i=1;i<=w;++i)
                if (tt<=a[i])
                {
                    printf("%.8lf\n",i-1+cal(uu[i-1]-ll[i-1],uu[i]-ll[i],1.0,tt));
                    break;
                }
                else tt-=a[i];
        }
    }
    return 0;
}
