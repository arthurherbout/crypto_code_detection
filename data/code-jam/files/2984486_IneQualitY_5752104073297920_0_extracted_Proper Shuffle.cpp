#include<cstdio>
int main()
{
    freopen("Proper Shuffer-small-attempt3.in","r",stdin);
    freopen("Proper Shuffer-small-attempt3.out","w",stdout);
    int T,n;
    scanf("%d",&T);
    for(int I=1,cnt=0;I<=T;I++)
    {
        scanf("%d",&n);
        cnt=0;
        for(int i=0,c;i<n;i++)
        {
            scanf("%d",&c);
            if(c==i)
                cnt++;
        }
        if(cnt>=1)
            printf("Case #%d: BAD\n",I);
        else
            printf("Case #%d: GOOD\n",I);
    }
}
