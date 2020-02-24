#include<bits/stdc++.h>
using namespace std;
int e[105],s[105];
int dis[105];
double ti[105];
int main()
{
freopen("C-small-attempt1.in","r",stdin);
freopen("c-s.out","w",stdout);
    int t;
    scanf("%d",&t);
    for(int cases=1;cases<=t;cases++)
    {
        int n,q;
        scanf("%d%d",&n,&q);
        for(int i=1;i<=n;i++)
        {
            scanf("%d%d",&e[i],&s[i]);
        }
        int a;
        for(int i=1;i<=n;i++)
        {
            for(int j=1;j<=n;j++)
            {
                scanf("%d",&a);
                if(j==i+1)dis[j]=a;
            }
        }
        int aa,aaa;
        scanf("%d%d",&aa,&aaa);
        ti[1]=0;
        for(int i=2;i<=n;i++)
        {
            ti[i]=1e12;
        }
        for(int i=1;i<n;i++)
        {
            long long nowdis=0;
            for(int j=i+1;j<=n;j++)
            {
                nowdis=nowdis+dis[j];
                if(nowdis>e[i])break;
                ti[j]=min(ti[j],ti[i]+1.0*nowdis/s[i]);
            }
        }
        printf("Case #%d: %.8lf\n",cases,ti[n]);
    }
    return 0;
}
