#include <iostream>
#include <cstdlib>
#include <cstdio>

using namespace std;

int main(void)
{
    freopen("A-large.in","r",stdin);
    freopen("A-large.out","w",stdout);
    int pd,pg,T,u,d;
    long long n;
    bool check;
    scanf("%d",&T);
    for(u=1;u<=T;++u)
    {
        scanf("%I64d%d%d",&n,&pd,&pg);
        check=false;
        for(d=1;d<=n;++d)
            if (d*pd%100==0)
            {
                check=true;
                break;
            }
        if ((pg==100)&&(pd!=100))
            check=false;
        if ((pg==0)&&(pd!=0))
            check=false;
        printf("Case #%d: ",u);
        if (check)
            puts("Possible");
        else puts("Broken");
    }
    return 0;
}
