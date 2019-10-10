#include <iostream>
#include <algorithm>
using namespace std;
int a[1010];
int main()
{
    freopen("d:\\A-small-attempt0.in.txt","r",stdin);
    freopen("d:\\out.txt","w",stdout);
    int i,n,ans,p,k,l,j,t,g,tot;
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
            ans+=tot;
            i=i-j;
        }
        printf("Case #%d: %d\n",t,ans);
    }
    return 0;
}