#include <iostream>
#include <cstdio>
#include <string.h>
#include <algorithm>
using namespace std;
#define MAXN 10000 + 10
struct Node
{
    long long d,l;
}p[MAXN];
bool cmp(Node a, Node b)
{
    return a.d<b.d;
}
long long dp[MAXN];
int main()
{
    freopen("t.in","r",stdin);
    freopen("t.txt","w",stdout);
    int t,n;
    long long td,tl,dd;
    scanf("%d",&t);
    bool ok;
    for(int z=1;z<=t;z++)
    {
        memset(dp,0,sizeof(dp));
        scanf("%d",&n);
        for(int i=0;i<n;i++)
            scanf("%lld%lld",&p[i].d, &p[i].l);
        scanf("%lld",&dd);
        //sort(p,p+n,cmp);
        dp[0]=2*p[0].d;
        ok=false;
        if(dp[0]>=dd)
            ok=true;
        for(int i=1;i<n&&ok==false;i++)
            for(int j=i-1;j>=0;j--)
                if(dp[j]>=p[i].d)
                {
                    dp[i]=max(dp[i], min(p[i].d-p[j].d,p[i].l)+p[i].d);
                    if(dp[i]>=dd)
                    {
                        ok=true;
                        break;
                    }
                }

        printf("Case #%d: ",z);
        if(ok)
            printf("YES\n");
        else
            printf("NO\n");
    }
	return 0;
}
