#include<cstdio>
#include<memory>

int h,i,j,n,m,t,s,i1,j1,i2,j2,x,y,a[111][111];

void go(int x,int y)
{
    if(x<0||y<0||x>=n||y>=m||a[x][y])return;
    a[x][y]=1;
    go(x+i1,y+j1);
    go(x+i2,y+j2);
}

int main()
{
    freopen("b.in","r",stdin);
    freopen("b.out","w",stdout);
    scanf("%d",&t);
    for(h=1;h<=t;h++)
    {
        scanf("%d%d%d%d%d%d%d%d",&n,&m,&i1,&j1,&i2,&j2,&x,&y);
        memset(a,0,sizeof(a));
        go(x,y);
        s=0;
        for(i=0;i<n;i++)
            for(j=0;j<m;j++)
                s+=a[i][j];
        printf("Case #%d: %d\n",h,s);
    }
    return 0;
}
