#include<stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
int main()
{
    int t,x,r,c,pos,t1;
    scanf("%d",&t);
    t1 = t;
    while(t--)
    {
        scanf("%d%d%d",&x,&r,&c);
        if(x==1)
            printf("Case #%d: GABRIEL\n",(t1-t));
        else
        {
            pos=r*c;
            if((pos>=(x*(x-1)))&&(pos%x==0))
                printf("Case #%d: GABRIEL\n",(t1-t));
            else
                printf("Case #%d: RICHARD\n",(t1-t));
        }
    }
    return 0;
}
