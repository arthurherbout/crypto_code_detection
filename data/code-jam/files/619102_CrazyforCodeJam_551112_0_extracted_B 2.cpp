#include "stdio.h"
#include "stdlib.h"
#include "string.h"
int main()
{
    freopen("B-small-attempt0.in","r",stdin);
    freopen("B-small-attempt0.out","w",stdout);
    
    int T,L,P,C;
    int i,j,k,res;
    
    scanf("%d",&T);
    
    for (i=0;i<T;i++)
    {
        res = 0;
        scanf("%d%d%d",&L,&P,&C);
        if (L*C>=P)
        printf("Case #%d: %d\n",i+1,0);
        else
        {
        for (j=0;1;j++)
        {
            int car = (P%C>0);
            P=((P)/(C))+car;res++;
            if (L*C>=P)break;
        }
        int rr=0;
        for (j=0;1;j++)
        {
            res=res/2;rr++;
            if (res==0)
            break;
        }
        printf("Case #%d: %d\n",i+1,rr);
        }
        
    }

return 0;
}
