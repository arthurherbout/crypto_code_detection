#include<stdio.h>

int main()
{
    freopen("B-large.in","r",stdin);
    freopen("B-large.out","w",stdout);
    short t,T;
    scanf("%hd",&T);
    for(t=1;t<=T;++t)
    {
        short N,S,p,n,ans;
        scanf("%hd %hd %hd",&N,&S,&p);
        ans=0;
        for(n=0;n<N;++n)
        {
            short score;
            scanf("%hd",&score);
            if(score<p)
                continue;
            if(score>=p*3-2)
            {
                ++ans;
                continue;
            }
            if(score>=p && score>=3*p-4 && S)
            {
                ++ans;
                --S;
            }
//            if(score<p*3-2)
//                continue;
        }
        printf("Case #%hd: %hd\n",t,ans);
    }
    return 0;
}
