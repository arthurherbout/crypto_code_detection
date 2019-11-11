#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>

using namespace std;

int T,U;
int n,a[1005],b[1005],arg,sum,f[1005],res;

int main(void)
{
    freopen("B-large.in","r",stdin);
    freopen("B-large.out","w",stdout);
    int i,j,k,t;
    scanf("%d",&T);
    for(U=1;U<=T;++U)
    {
        scanf("%d",&n);
        for(i=1;i<=n;++i)
        {
            scanf("%d%d",a+i,b+i);
            f[i]=0;
        }
        
        for(i=1;i<n;++i)
            for(j=i+1;j<=n;++j)
                if (b[i]-a[i]<b[j]-a[j])
                {
                    t=a[i];
                    a[i]=a[j];
                    a[j]=t;
                    
                    t=b[i];
                    b[i]=b[j];
                    b[j]=t;
                }
        
        sum=res=0;
        while(true)
        {
            arg=0;
            for(i=1;i<=n;++i)
            {
                if (arg<1-f[i] && f[i]<1 && a[i]<=sum)
                {
                    arg=1-f[i]; j=i; k=1;
                }
                if (arg<=2-f[i] && f[i]<2 && b[i]<=sum)
                {
                    arg=2-f[i]; j=i; k=2;
                }
            }
            if (arg==0)
                break;
            sum+=arg;
            ++res;
            f[j]=k;
        }
        printf("Case #%d: ",U);
        if (sum==n+n)
            printf("%d\n",res);
        else puts("Too Bad");
    }
    return 0;
}
