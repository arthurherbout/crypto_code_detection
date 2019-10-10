#include "stdio.h"
#include "stdlib.h"
#include "string.h"
int main()
{
    freopen("B-small-attempt0.in","r",stdin);
    freopen("B-large.out","w",stdout);
    
    long long T,L,P,C;
    long long i,j,k,res;
    
    scanf("%lld",&T);
    
    for (i=0;i<T;i++)
    {
        res = 0;
        scanf("%lld%lld%lld",&L,&P,&C);
        if (L*C>=P)
        printf("Case #%lld: %lld\n",i+1,0);
        else
        {
        for (j=0;1;j++)
        {
            int car = (P%C>0);
            P=((P)/(C))+car;res++;
            if (L*C>=P)break;
        }
        long long  rr=0;
        for (j=0;1;j++)
        {
            res=res/2;rr++;
            if (res==0)
            break;
        }
        printf("Case #%lld: %lld\n",i+1,rr);
        }
        
    }

return 0;
}
