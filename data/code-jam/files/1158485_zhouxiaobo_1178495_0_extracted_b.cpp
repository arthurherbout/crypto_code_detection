#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <cmath>

using namespace std;

int f[30005],g[30005];
int n,a[1005];

bool check(int k)
{
    int i,j;
    for(i=1;i<=10000;++i)
    {
        f[i]=0;
        g[i]=0;
    }
    for(i=1;i<=n;++i)
        ++f[a[i]];
    for(i=1;i<=10000;++i)
    {
        while(f[i]>0)
        {
            bool b=true;
            for(j=i;j<i+k;++j)
                if (f[j]==0)
                {
                    b=false;
                    break;
                }
            if (b)
            {
                for(j=i;j<i+k;++j)
                    --f[j];
                ++g[i+k];
            }
            else break;
        }
        if (f[i]>g[i]) return false;
        g[i+1]+=f[i];
    }
    return true;
}

int main(void)
{
    freopen("B-small-attempt0.in","r",stdin);
    freopen("B-small-attempt0.out","w",stdout);
    int T,u,res,ll,rr,t,i;
    scanf("%d",&T);
    for(u=1;u<=T;++u)
    {
        scanf("%d",&n);
        if (n==0)
        {
            printf("Case #%d: %d\n",u,0);
            continue;
        }
        for(i=1;i<=n;++i)
        {
            scanf("%d",&t);
            a[i]=t;
        }
        ll=1;
        rr=n+1;
        while(ll+1<rr)
        {
            t=(ll+rr)/2;
            if (check(t))
                ll=t;
            else rr=t;
        }
        printf("Case #%d: %d\n",u,ll);
    }
    return 0;
}
