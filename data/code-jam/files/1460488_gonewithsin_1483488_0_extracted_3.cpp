#include<iostream>
#include<string.h>
#include<stdio.h>
using namespace std;

int n,a,b,minn,maxn,t,u,num,g,ans;
int f[2000005];

int fac(int x,int y)
{
    int a,b,c,d;
    a=x%10;
    y--;
    while(y)y--,a*=10;
    return x/10+a;
}

int main()
{

    freopen("C-small-attempt0.in","r",stdin);
    freopen("out.out","w",stdout);
    scanf("%d",&n);
    for(int k=1;k<=n;k++)
    {
        memset(f,0,sizeof(f));
        scanf("%d%d",&minn,&maxn);
        num=0;
        g=minn;
        ans=0;
        while(g)g/=10,num++;

        for(int i=minn;i<=maxn;i++)
        {
            t=i;
            f[t]=i;
            for(int j=1;j<num;j++)
            {
                u=fac(t,num);
                if(u>=minn && u<=maxn && u>i && f[u]<i)f[u]=i,ans++;
                t=u;
            }
        }
        printf("Case #%d: %d\n",k,ans);
    }
    return 0;
}
