#include<cstdio>
#include<memory>

int h,i,j,k,n,m,t,x,y,tx,ty,s,u[999],v[999],b[33][33],c[33][33],f[33][33],g[33][33];
char a[33][33];

void go(int x,int y)
{
    if(f[x][y]>-1||a[x][y]=='.')return;
    f[x][y]=u[j];g[x][y]=v[j];
    k++;u[k]=x;v[k]=y;
    c[x][y]=c[u[j]][v[j]]+1;
}

int main()
{
    freopen("d.in","r",stdin);
    freopen("d.out","w",stdout);
    scanf("%d",&t);
    for(h=1;h<=t;h++)
    {
        scanf("%d%d",&n,&m);
        tx=-1;ty=-1;
        for(i=0;i<n;i++)
        {
            scanf(" %s",a[i]);
            for(j=0;j<m;j++)
                if(i+j>0&&a[i][j]=='T')
                {
                    tx=i;ty=j;
                }
        }
        u[0]=0;v[0]=0;j=0;k=0;
        memset(f,-1,sizeof(f));
        memset(c,0,sizeof(c));
        f[0][0]=0;
        while(j<=k)
        {
            x=u[j];y=v[j];
            if(x)go(x-1,y);
            if(y)go(x,y-1);
            if(x<n-1)go(x+1,y);
            if(y<m-1)go(x,y+1);
            j++;
        }
        memset(b,-1,sizeof(b));
        if(tx>-1)
        {
            x=tx;y=ty;i=0;b[x][y]=0;
            while(x+y>0)
            {
                j=f[x][y];k=g[x][y];
                x=j;y=k;i++;b[x][y]=i;
            }
            u[0]=0;v[0]=0;
            u[1]=tx;v[1]=ty;
            j=0;k=1;
            memset(f,-1,sizeof(f));
            memset(c,0,sizeof(c));
            f[0][0]=0;f[tx][ty]=0;
            while(j<=k)
            {
                x=u[j];y=v[j];
                if(x)go(x-1,y);
                if(y)go(x,y-1);
                if(x<n-1)go(x+1,y);
                if(y<m-1)go(x,y+1);
                j++;
            }
        }
        s=0;
        for(i=0;i<n;i++)
            for(j=0;j<m;j++)
                if(b[i][j]>-1)s+=b[i][j];
                else s+=c[i][j];
        printf("Case #%d: %d\n",h,s);
    }
    return 0;
}
