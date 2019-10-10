#include <iostream>
#include <cstdlib>
#include <cstdio>

using namespace std;

const int SIZE=505;

struct board2 //¶þÎ¬Ê÷×´Êý×é 
{
    int n,m,c[SIZE][SIZE];
    void init(int n0,int m0)
    {
        n=n0; m=m0;
        int i,j;
        for(i=1;i<=n;++i)
            for(j=1;j<=m;++j)
                c[i][j]=0;
    }
    void add(int i,int j,int v)
    {
        int x,y;
        for(x=i;x<=n;x+=(-x)&(x))
            for(y=j;y<=m;y+=(-y)&(y))
                c[x][y]+=v;
    }
    int sum(int i,int j)
    {
        int x,y,res=0;
        for(x=i;x>0;x-=(-x)&(x))
            for(y=j;y>0;y-=(-y)&(y))
                res+=c[x][y];
        return res;
    }
    int sum(int x1,int y1,int x2,int y2)
    { return sum(x2,y2)-sum(x2,y1-1)-sum(x1-1,y2)+sum(x1-1,y1-1); }
}xsum,ysum,fsum;

int a[SIZE][SIZE];

int main(void)
{
    freopen("B-large.in","r",stdin);
    freopen("B-large.out","w",stdout);
    int T,u,n,m,d,res,i,j,k;
    scanf("%d",&T);
    char st[505];
    for(u=1;u<=T;++u)
    {
        res=2;
        scanf("%d%d%d",&n,&m,&d);
        xsum.init(n,m);
        ysum.init(n,m);
        fsum.init(n,m);
        for(i=1;i<=n;++i)
        {
            scanf("%s",st);
            for(j=1;j<=m;++j)
            {
                a[i][j]=st[j-1]-'0';
                xsum.add(i,j,i*a[i][j]);
                ysum.add(i,j,j*a[i][j]);
                fsum.add(i,j,a[i][j]);
            }
        }
        for(i=1;i<=n;++i)
            for(j=1;j<=m;++j)
                for(k=3;i+k-1<=n&&j+k-1<=m;++k)
                {
                    if (((xsum.sum(i,j,i+k-1,j+k-1)
                      -a[i][j]*i-a[i][j+k-1]*i-a[i+k-1][j]*(i+k-1)-a[i+k-1][j+k-1]*(i+k-1))*2==
                        (fsum.sum(i,j,i+k-1,j+k-1)
                      -a[i][j]-a[i][j+k-1]-a[i+k-1][j]-a[i+k-1][j+k-1])*(i+i+k-1))
                     &&((ysum.sum(i,j,i+k-1,j+k-1)
                      -a[i][j]*j-a[i][j+k-1]*(j+k-1)-a[i+k-1][j]*j-a[i+k-1][j+k-1]*(j+k-1))*2==
                        (fsum.sum(i,j,i+k-1,j+k-1)
                      -a[i][j]-a[i][j+k-1]-a[i+k-1][j]-a[i+k-1][j+k-1])*(j+j+k-1)))
                         res=max(k,res);
                }
        if (res<3)
            printf("Case #%d: IMPOSSIBLE\n",u);
        else printf("Case #%d: %d\n",u,res);
    }
    return 0;
}
