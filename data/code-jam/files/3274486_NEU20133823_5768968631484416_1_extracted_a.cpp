#include<bits/stdc++.h>
using namespace std;
const double pi=acos(-1.0);
struct pan
{
    long long r,h;
    long long sc;
};
pan p[1005];
bool cmp(pan a,pan b)
{
    return a.sc>b.sc;
}
int main()
{
    freopen("A-large.in","r",stdin);
    freopen("a-b.out","w",stdout);
    int t;
    scanf("%d",&t);
    for(int cases=1;cases<=t;cases++)
    {
        int n,k;
        scanf("%d%d",&n,&k);
        for(int i=0;i<n;i++)
        {
            scanf("%lld%lld",&p[i].r,&p[i].h);
            p[i].sc=2*p[i].r*p[i].h;
        }
    sort(p,p+n,cmp);
    long long ans=0;
    long long base=0;
    long long maxr=0;
    for(int i=0;i<k;i++)
    {
        base=base+p[i].sc;
        maxr=max(maxr,p[i].r);
    }
    base=base+maxr*maxr;
    ans=base;
    //printf("%lld %lf\n",ans,pi*ans);
    for(int i=k;i<n;i++)
    {
        if(p[i].r>maxr)
        {
            //printf("%lld %lld\n",p[i].r,p[i].h);
            ans=max(ans,base-p[k-1].sc+p[i].sc-maxr*maxr+p[i].r*p[i].r);
            //printf("%lld %lf\n",base-p[k-1].sc+p[i].sc-maxr*maxr+p[i].r*p[i].r,pi*(base-p[k-1].sc+p[i].sc-maxr*maxr+p[i].r*p[i].r));
        }
    }
    printf("Case #%d: %.9lf\n",cases,ans*pi);
    }
    return 0;
}
