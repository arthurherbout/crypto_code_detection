#include <iostream>
#include <cstdlib>
#include <cstdio>

using namespace std;

double f[1001];
int a[1001];

int main(void)
{
    double c=0.0,res;
    int i,j,n,u,T,m,t;
    f[1]=0.0;
    for(n=2;n<=1000;++n)
    {
        f[n]=(n+c)/(n-1.0);
        c+=f[n]+f[n]-1.0;
    }
    freopen("D-large.in","r",stdin);
    freopen("D-large.out","w",stdout);
    scanf("%d",&T);
    for(u=1;u<=T;++u)
    {
        scanf("%d",&n);
        for(i=1;i<=n;++i)
            scanf("%d",a+i);
        res=0.0;
        for(i=1;i<=n;++i)
            if (a[i])
            {
                m=0;
                for(j=i;a[j];t=a[j],a[j]=0,j=t)
                    ++m;
                res+=f[m];
            }
        printf("Case #%d: %.6lf\n",u,res);
    }
    return 0;
}
