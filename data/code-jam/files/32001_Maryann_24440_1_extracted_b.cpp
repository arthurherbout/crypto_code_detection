#include<cstdio>
int n,m,a,w;
void work()
{
    if(n*m>=a)
    {
        for(int x1=n; x1>=0; x1--)
        for(int y1=m; y1>=0; y1--)
        if(x1*y1>=a)
        {
            w=x1*y1-a;
            if(w==0)
            {
                printf("0 0 0 %d %d 0\n",y1,x1);
                return;
            }
            for(int x2=w/m; x2<=n; x2++)
            {
                if(x2==0)continue;
                if(w%x2==0 && w/x2<=m)
                {
                    printf("0 0 %d %d %d %d\n",x1,w/x2,x2,y1);
                    return;
                }
            }
        }
        else
        break;
    }
    puts("IMPOSSIBLE");
}
int main()
{
    int _,t;
    freopen("b.in","r",stdin);
    freopen("b2.out","w",stdout);
    scanf("%d",&_);
    for(t=1; t<=_; t++)
    {
        scanf("%d%d%d",&n,&m,&a);
        printf("Case #%d: ",t);
        work();
    }
    return 0;
}