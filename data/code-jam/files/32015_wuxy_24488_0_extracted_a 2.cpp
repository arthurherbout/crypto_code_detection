#include <iostream>
#include <algorithm>
using namespace std;
long long a[128],b[1010],c[1010];
long long x,y,z;
long long ans;
int main()
{
    freopen("d:\\C-small-attempt1.in.txt","r",stdin);
    freopen("d:\\out.txt","w",stdout);
    int i,j,t,tt,m,n;
    scanf("%d",&tt);
    for (t=1;t<=tt;t++)
    {
        scanf("%d%d%lld%lld%lld",&n,&m,&x,&y,&z);
        for (i=0;i<m;i++)
            scanf("%lld",a+i);
        x=x%z;
        y=y%z;
        for (i=0;i<n;i++)
        {
            b[i]=a[i%m];
            a[i%m]=((x*a[i%m])%z+(y*(i+1))%z)%z;
        }
        ans=0;
        for (i=0;i<n;i++)
        {
            c[i]=1;
            for (j=0;j<i;j++)
                if (b[i]>b[j])
                    c[i]=(c[i]+c[j])%1000000007;
            ans=(ans+c[i])%1000000007;
        }
        printf("Case #%d: %lld\n",t,ans);
    }
    return 0;
}