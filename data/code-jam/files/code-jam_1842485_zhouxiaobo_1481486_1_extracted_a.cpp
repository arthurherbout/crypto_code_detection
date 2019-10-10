#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <string>
#include <cmath>
#include <algorithm>

using namespace std;

int T,U;

int f[10005];
int d[10005];
int ll[10005];
int q[100005];
bool b[10005];
int n,L;

bool solve()
{
    int l,r,i,j,k;
    l=0; r=1;
    q[0]=1;
    b[1]=true;
    while(l!=r)
    {
        k=q[l];
        if (f[k]+d[k]>=L)
            return true;
        for(i=k-1;i>=1;--i)
        {
            int t=d[k]-d[i];
            if (t>f[k])
                break;
            t=min(t,ll[i]);
            if (f[i]<t)
            {
                f[i]=t;
                if (!b[i])
                {
                    b[i]=true;
                    q[r]=i;
                    r=((r+1)&65535);
                }
            }
        }
        for(i=k+1;i<=n;++i)
        {
            int t=d[i]-d[k];
            if (t>f[k])
                break;
            t=min(t,ll[i]);
            if (f[i]<t)
            {
                f[i]=t;
                if (!b[i])
                {
                    b[i]=true;
                    q[r]=i;
                    r=((r+1)&65535);
                }
            }
        }
        b[k]=false;
        l=((l+1)&65535);
    }
    return false;
}


int main(void)
{
    freopen("A-large.in","r",stdin);
    freopen("A-large.out","w",stdout);

    scanf("%d",&T);
    int i,j;
    for(U=1;U<=T;++U)
    {
        scanf("%d",&n);
        for(i=1;i<=n;++i)
        {
            scanf("%d%d",d+i,ll+i);
            f[i]=0;
            b[i]=false;
        }
        scanf("%d",&L);
        f[1]=d[1];
        printf("Case #%d: %s\n",U,(solve())?"YES":"NO");
    }
    
    //system("PAUSE");
    return 0;
}
