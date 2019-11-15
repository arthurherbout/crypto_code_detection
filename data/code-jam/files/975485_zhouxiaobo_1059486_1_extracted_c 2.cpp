#include <iostream>
#include <cstdlib>
#include <cstdio>

using namespace std;

int c[1001];

int main(void)
{
    freopen("C-large.in","r",stdin);
    freopen("C-large.out","w",stdout);
    int u,T,n,i,j;
    int sum,xorsum,minn;
    scanf("%d",&T);
    for(u=1;u<=T;++u)
    {
        scanf("%d",&n);
        xorsum=sum=0;
        minn=1000000008;
        for(i=1;i<=n;++i)
        {
            scanf("%d",c+i);
            sum+=c[i];
            xorsum^=c[i];
            minn=min(minn,c[i]);
        }
        if (xorsum==0)
            printf("Case #%d: %d\n",u,sum-minn);
        else printf("Case #%d: NO\n",u);
    }
    return 0;
}
