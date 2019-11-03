#include<cstdio>

int h,i,j,k,l,n,s,m,t,a[5555],b[5555],c[5555];

int main()
{
    freopen("a.in","r",stdin);
    freopen("a.out","w",stdout);
    scanf("%d",&t);
    for(h=1;h<=t;h++)
    {
        scanf("%d",&n);
        for(i=0;i<n;i++)
            scanf("%d%d%d",&a[i],&b[i],&c[i]);
        m=0;
        for(i=0;i<=10000;i++)
        for(j=0;i+j<=10000;j++)
        {
            k=10000-i-j;
            s=0;
            for(l=0;l<n;l++)
                if(i>=a[l]&&j>=b[l]&&k>=c[l])s++;
            if(s>m)m=s;
        }
        printf("Case #%d: %d\n",h,m);
    }
    return 0;
}
