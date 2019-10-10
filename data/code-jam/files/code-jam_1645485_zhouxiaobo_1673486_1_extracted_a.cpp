#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>

using namespace std;

int T,a,b,U;
double p[100005],res,t,cur;

int main(void)
{
    freopen("A-large.in","r",stdin);
    freopen("A-large.out","w",stdout);
    int i;
    scanf("%d",&T);
    for(U=1;U<=T;++U)
    {
        scanf("%d%d",&a,&b);
        for(i=1;i<=a;++i)
            scanf("%lf",p+i);
        res=1+b+1;
        t=1.0;
        for(i=1;i<=a;++i)
        {
            t*=p[i];
            res=min(res,(a-i+b-i+1)+(1-t)*(b+1));
        }
        printf("Case #%d: %.6lf\n",U,res);
    }
    return 0;
}
