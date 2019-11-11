#include <iostream>
#include <algorithm>
using namespace std;
int a[1010];
int main()
{
    freopen("d:\\A-large.in.txt","r",stdin);
    freopen("d:\\out.txt","w",stdout);
    int i,n,p,k,l,j,t,g,tot;
    long long ans;
    scanf("%d",&n);
    for (t=1;t<=n;t++)
    {
        ans=0;
        scanf("%d%d%d",&p,&k,&l);
        for (j=0;j<l;j++)
            scanf("%d",a+j);
        if (p*k<l)
        {
            printf("Impossible\n");
            continue;
        }
        sort(a,a+l);
        g=0;
        for (i=l-1;i>=0;)
        {
            g++;
            tot=0;
            for (j=0;i-j>=0&&j<k;j++)
                tot+=a[i-j];
            tot*=g;
            ans=ans+tot;
            i=i-j;
        }
        printf("Case #%d: %lld\n",t,ans);
    }
    return 0;
}