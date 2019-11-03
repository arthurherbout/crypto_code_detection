#include<iostream>
#include<string.h>
#include<stdio.h>
using namespace std;

int main()
{
    //
    freopen("B-large.in","r",stdin);
    //
    freopen("out.out","w",stdout);
    int n,t,s,p,ans;
    int a[300];
    bool b[300];
    scanf("%d",&n);
    for(int k=1;k<=n;k++)
    {
        memset(a,0,sizeof(a));
        memset(b,0,sizeof(b));
        scanf("%d%d%d",&t,&s,&p);
        for(int j=1;j<=t;j++)
        {
            scanf("%d",&a[j]);
            if(a[j]%3==1)b[j]=0;
                else if(a[j]<=28 && a[j]>=2)b[j]=1;
                    else b[j]=0;
            if(a[j]%3)a[j]=a[j]/3+1;
                else a[j]/=3;
        }
        ans=0;
        for(int i=1;i<=t;i++)
            if(a[i]>=p)ans++;
                else if(a[i]+1==p && b[i] && s>0)s--,ans++;
        printf("Case #%d: %d\n",k,ans);
    }
    return 0;
}
