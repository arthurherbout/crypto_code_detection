#include <iostream>
#include <cstdlib>
#include <cstdio>

using namespace std;

int a[1000005];

int main(void)
{
    freopen("A-large.in","r",stdin);
    freopen("A-large.out","w",stdout);
    int T,s,r,u,n,X,i,j,x,y,c;
    double t,tt;
    scanf("%d",&T);
    for(u=1;u<=T;++u)
    {
        scanf("%d%d%d%lf%d",&X,&s,&r,&t,&n);
        for(i=1;i<=X;++i)
            a[i]=s;
        for(i=1;i<=n;++i)
        {
            scanf("%d%d%d",&x,&y,&c);
            for(j=x+1;j<=y;++j)
                a[j]+=c;
        }
        sort(a+1,a+X+1);
        double res=0.0;
        for(i=1;i<=X;++i)
        {
            if (t>1e-10)
            {
                tt=1.0/(a[i]+r-s);
                if (tt<t)
                {
                    t-=tt;
                    res+=tt;
                }
                else
                {
                    res+=t+(1.0-(a[i]+r-s)*t)/a[i];
                    t=0.0;
                }
            }
            else res+=1.0/a[i];
        }
        printf("Case #%d: %.8lf\n",u,res);
    }
    return 0;
}
