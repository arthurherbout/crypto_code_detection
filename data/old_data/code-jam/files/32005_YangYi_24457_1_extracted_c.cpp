#include<cstdio>

double a[15][32769],p;
int h,i,j,n,t,x;

double f(int n,int x)
{
    int i,l,r,o,q;
    double m,u,v;
    if(x>32768)return 1;
    if(!n)
    {
        if(x+x>32768)return p;
        else return 0;
    }
    if(!x)return 0;
    if(a[n][x]>-1)return a[n][x];
    m=0;
    l=0;r=x;
    if(r+r>32768)r=32769-x;
    while(l+2<r)
    {
        o=l+(r-l+2)/3;
        q=r-(r-l+2)/3;
        u=f(n-1,x+o)*p+f(n-1,x-o)*(1-p);
        v=f(n-1,x+q)*p+f(n-1,x-q)*(1-p);
        if(u>m)m=u;
        if(v>m)m=v;
        if(u>v)r=q;
        else l=o;
    }
    a[n][x]=m;
    return m;
}

int main()
{
    freopen("c.in","r",stdin);
    freopen("c.out","w",stdout);
    scanf("%d",&t);
    for(h=0;h<t;h++)
    {
        scanf("%d %lf %d",&n,&p,&x);
        for(i=0;i<15;i++)
            for(j=0;j<32769;j++)a[i][j]=-2;
        for(i=0;i<n;i++)
            for(j=0;j<32769;j++)
                a[i][j]=f(i,j);
        x=(long long)(x)*32768/1000000;
        printf("Case #%d: %.6lf\n",h+1,a[n-1][x]);
    }
    return 0;
}
