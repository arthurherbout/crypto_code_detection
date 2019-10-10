#include <iostream>
#include <algorithm>
using namespace std;
#define N 10010
int a[N][2],x[2],y[2];
bool g[N],c[N];
int main()
{
    freopen("d:\\A-large.in.txt","r",stdin);
    freopen("d:\\out.txt","w",stdout);
    int i,j,t,tt,m,n,v,k;
    scanf("%d",&tt);
    for (t=1;t<=tt;t++)
    {
        scanf("%d%d",&m,&v);
        for (i=1;i<=m/2;i++)
            scanf("%d%d",g+i,c+i);
        for (;i<=m;i++)
        {
            scanf("%d",&j);
            a[i][j]=0;
            a[i][1-j]=-1;
        }
        for (i=m/2;i>0;i--)
        {
            if (c[i])
            {
                    if (a[i*2][1]>=0&&a[i*2+1][1]>=0)
                        x[1]=a[i*2][1]+a[i*2+1][1];
                    else
                        x[1]=-1;
                    k=m+1;
                    if (a[i*2][0]>=0&&a[i*2+1][0]>=0)
                    {
                        if (k>a[i*2][0]+a[i*2+1][0])
                            k=a[i*2][0]+a[i*2+1][0];
                    }
                    if (a[i*2][0]>=0&&a[i*2+1][1]>=0)
                    {
                        if (k>a[i*2][0]+a[i*2+1][1])
                            k=a[i*2][0]+a[i*2+1][1];
                    }
                    if (a[i*2][1]>=0&&a[i*2+1][0]>=0)
                    {
                        if (k>a[i*2][1]+a[i*2+1][0])
                            k=a[i*2][1]+a[i*2+1][0];
                    }
                    if (k==m+1)
                        x[0]=-1;
                    else
                        x[0]=k;

                    if (a[i*2][0]>=0&&a[i*2+1][0]>=0)
                        y[0]=a[i*2][0]+a[i*2+1][0];
                    else
                        y[0]=-1;
                    k=m+1;
                    if (a[i*2][1]>=0&&a[i*2+1][1]>=0)
                    {
                        if (k>a[i*2][1]+a[i*2+1][1])
                            k=a[i*2][1]+a[i*2+1][1];
                    }
                    if (a[i*2][0]>=0&&a[i*2+1][1]>=0)
                    {
                        if (k>a[i*2][0]+a[i*2+1][1])
                            k=a[i*2][0]+a[i*2+1][1];
                    }
                    if (a[i*2][1]>=0&&a[i*2+1][0]>=0)
                    {
                        if (k>a[i*2][1]+a[i*2+1][0])
                            k=a[i*2][1]+a[i*2+1][0];
                    }
                    if (k==m+1)
                        y[1]=-1;
                    else
                        y[1]=k;


                if (g[i])
                {
                    if (x[0]>=0&&y[0]>=0)
                    {
                        if (x[0]<y[0]+1)
                            a[i][0]=x[0];
                        else
                            a[i][0]=y[0]+1;
                    }
                    else if (x[0]<0&&y[0]<0)
                        a[i][0]=-1;
                    else if (x[0]>=0)
                        a[i][0]=x[0];
                    else
                        a[i][0]=y[0]+1;

                    if (x[1]>=0&&y[1]>=0)
                    {
                        if (x[1]<y[1]+1)
                            a[i][1]=x[1];
                        else
                            a[i][1]=y[1]+1;
                    }
                    else if (x[1]<0&&y[1]<0)
                        a[i][1]=-1;
                    else if (x[1]>=0)
                        a[i][1]=x[1];
                    else
                        a[i][1]=y[1]+1;

                }
                else
                {
                    if (x[0]>=0&&y[0]>=0)
                    {
                        if (y[0]<x[0]+1)
                            a[i][0]=y[0];
                        else
                            a[i][0]=x[0]+1;
                    }
                    else if (x[0]<0&&y[0]<0)
                        a[i][0]=-1;
                    else if (x[0]>=0)
                        a[i][0]=x[0]+1;
                    else
                        a[i][0]=y[0];

                    if (x[1]>=0&&y[1]>=0)
                    {
                        if (y[1]<x[1]+1)
                            a[i][1]=y[1];
                        else
                            a[i][1]=x[1]+1;
                    }
                    else if (x[1]<0&&y[1]<0)
                        a[i][1]=-1;
                    else if (x[1]>=0)
                        a[i][1]=x[1]+1;
                    else
                        a[i][1]=y[1];
                }
            }
            else
            {
                if (g[i])
                {
                    if (a[i*2][1]>=0&&a[i*2+1][1]>=0)
                        a[i][1]=a[i*2][1]+a[i*2+1][1];
                    else
                        a[i][1]=-1;
                    k=m+1;
                    if (a[i*2][0]>=0&&a[i*2+1][0]>=0)
                    {
                        if (k>a[i*2][0]+a[i*2+1][0])
                            k=a[i*2][0]+a[i*2+1][0];
                    }
                    if (a[i*2][0]>=0&&a[i*2+1][1]>=0)
                    {
                        if (k>a[i*2][0]+a[i*2+1][1])
                            k=a[i*2][0]+a[i*2+1][1];
                    }
                    if (a[i*2][1]>=0&&a[i*2+1][0]>=0)
                    {
                        if (k>a[i*2][1]+a[i*2+1][0])
                            k=a[i*2][1]+a[i*2+1][0];
                    }
                    if (k==m+1)
                        a[i][0]=-1;
                    else
                        a[i][0]=k;
                }
                else
                {
                    if (a[i*2][0]>=0&&a[i*2+1][0]>=0)
                        a[i][0]=a[i*2][0]+a[i*2+1][0];
                    else
                        a[i][0]=-1;
                    k=m+1;
                    if (a[i*2][1]>=0&&a[i*2+1][1]>=0)
                    {
                        if (k>a[i*2][1]+a[i*2+1][1])
                            k=a[i*2][1]+a[i*2+1][1];
                    }
                    if (a[i*2][0]>=0&&a[i*2+1][1]>=0)
                    {
                        if (k>a[i*2][0]+a[i*2+1][1])
                            k=a[i*2][0]+a[i*2+1][1];
                    }
                    if (a[i*2][1]>=0&&a[i*2+1][0]>=0)
                    {
                        if (k>a[i*2][1]+a[i*2+1][0])
                            k=a[i*2][1]+a[i*2+1][0];
                    }
                    if (k==m+1)
                        a[i][1]=-1;
                    else
                        a[i][1]=k;
                }
            }
        }
        if (a[1][v]>=0)
            printf("Case #%d: %d\n",t,a[1][v]);
        else
            printf("Case #%d: IMPOSSIBLE\n",t); 
    }
    return 0;
}