#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cmath>
#include<algorithm>
using namespace std;

long long at[1000],an[1000],bt[1000],bn[1000],n,m;

long long find(long long at1,long long an1,long long bt1,long long bn1,int a2,int b2)
{
    if(a2==n+2||b2==m+2) return 0;
   if(at1==bt1)
   {
       if(an1==bn1)
           return an1+find(at[a2],an[a2],bt[b2],bn[b2],a2+1,b2+1);
       if(an1<bn1)
           return an1+find(at[a2],an[a2],bt1,bn1-an1,a2+1,b2);
       if(an1>bn1)
           return bn1+find(at1,an1-bn1,bt[b2],bn[b2],a2,b2+1);
    }
   else
   {
       long long ans1,ans2;
       ans1=find(at1,an1,bt[b2],bn[b2],a2,b2+1);
       ans2=find(at[a2],an[a2],bt1,bn1,a2+1,b2);
       if(ans1>ans2)
           return ans1;
       return ans2;
   }
}

int main()
{
    freopen("4.in","r",stdin);
    freopen("4.out","w",stdout);
    int cas,g;
    scanf("%d",&cas);
    for(g=1;g<=cas;g++)
    {
        scanf("%d%d",&n,&m);
        for(int i=1;i<=n;i++)
            scanf("%I64d%I64d",&an[i],&at[i]);
        for(int i=1;i<=m;i++)
            scanf("%I64d%I64d",&bn[i],&bt[i]);
        long long ans=0;
        ans=find(at[1],an[1],bt[1],bn[1],2,2);
        printf("Case #%d: %I64d\n",g,ans);

    }
    return 0;
}