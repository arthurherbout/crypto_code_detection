#include <iostream>
#include <cstdio>
#include <string.h>
#include <algorithm>
#include <cmath>
using namespace std;
#define MAXN 1000 + 10

double rr[1100],x[1100],y[1100];

double findx(int k,double l,double r)
{
    double m;
    bool ok;
    while(fabs(r-l)>1e-6)
    {
        m=(l+r)/2.0;
        ok=true;
        for(int i=0;i<k;i++)
            if(m-x[i]<rr[k]+rr[i])
            {
                ok=false;
                break;
            }
        if(ok)
            r=m;
        else
            l=m;
    }
    return m;
}
double findy(int k,double l,double r)
{
    double m;
    bool ok;
    while(fabs(r-l)>1e-6)
    {
        m=(l+r)/2.0;
        ok=true;
        for(int i=0;i<k;i++)
            if(m-y[i]<rr[k]+rr[i])
            {
                ok=false;
                break;
            }
        if(ok)
            r=m;
        else
            l=m;
    }
    return m;
}
int main()
{
    freopen("t.in","r",stdin);
    freopen("t.txt","w",stdout);
    int t,n;
    double w,l;
    scanf("%d",&t);
    for(int z=1;z<=t;z++)
    {
        scanf("%d%lf%lf",&n,&w,&l);
        for(int i=0;i<n;i++)
            scanf("%lf",&rr[i]);
        x[0]=y[0]=0.0;
        for(int i=1;i<n;i++)
        {
            x[i]=findx(i,0,w);
            y[i]=findy(i,0,l);
        }
        printf("Case #%d:",z);
        for(int i=0;i<n;i++)
            printf(" %.6lf %.6lf",x[i],y[i]);
        printf("\n");
    }
	return 0;
}
