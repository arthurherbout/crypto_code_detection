#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <string>
#include <cmath>
#include <algorithm>

using namespace std;

int T,U;

int n;
int x[2005];
int res[2005];

bool flag;

void solve(int l,int r,int p)
{
    int i,j,t,ll=l;
    while(l<=r)
    {
        t=x[l];
        int d=(t-l)*p;
        for(i=ll;i<t;++i)
            res[i]-=d;
        --p;
        for(i=l;i<t;++i)
        {
            if (x[i]==t) continue;
            for(j=i;x[j]!=t && j<t;++j);
            if (j==t)
            {
                flag=false;
                return;
            }
            solve(i,j-1,p+20000);
            if (!flag) return;
            i=j;
        }
        l=t;
    }
}

int main(void)
{
    freopen("C-small-attempt4.in","r",stdin);
    freopen("C-small-attempt4.out","w",stdout);
    int i;
    scanf("%d",&T);
    for(U=1;U<=T;++U)
    {
        scanf("%d",&n);
        for(i=1;i<n;++i)
        {
            scanf("%d",x+i);
            res[i]=0;
        }
        res[n]=0;
        flag=true;
        solve(1,n-1,10000);
        if (!flag)
        {
            printf("Case #%d: Impossible\n",U);
            continue;
        }
        int dmin=0;
        for(i=1;i<=n;++i)
            dmin=min(dmin,res[i]);
         printf("Case #%d:",U);
         for(i=1;i<=n;++i)
             printf(" %d",res[i]-dmin);
         printf("\n");
    }
    
    return 0;
}
