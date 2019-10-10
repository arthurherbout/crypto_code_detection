#include<cstdio>

int h,i,j,n,m,s,o,t,a[55][55],b[55][55];

void tr(int x,int y)
{
    int i,j,k;
    if(x>=n)
    {
        for(i=0;i<n;i++)
        for(j=0;j<m;j++)
        if(a[i][j])return;
        s=0;
        for(i=0;i<m;i++)
            s+=b[n/2][i];
        if(s>o)o=s;
        return;
    }
    if(y>=m)
    {
        tr(x+1,0);
        return;
    }
    tr(x,y+1);
    k=1;
    for(i=x-1;i<x+2;i++)
    if(i>=0&&i<n)
        for(j=y-1;j<y+2;j++)
        if(j>=0&&j<m)
        {
            a[i][j]--;
            if(a[i][j]<0)k=0;
        }
    if(k)
    {
        b[x][y]=1;
        tr(x,y+1);
        b[x][y]=0;
    }
    for(i=x-1;i<x+2;i++)
    if(i>=0&&i<n)
        for(j=y-1;j<y+2;j++)
        if(j>=0&&j<m)a[i][j]++;
 }

int main()
{
    freopen("c.in","r",stdin);
    freopen("c.out","w",stdout);
    scanf("%d",&t);
    for(h=1;h<=t;h++)
    {
        scanf("%d%d",&n,&m);
        for(i=0;i<n;i++)
            for(j=0;j<m;j++)
                scanf("%d",&a[i][j]);
        o=0;tr(0,0);
        printf("Case #%d: %d\n",h,o);
    }
    return 0;
}
