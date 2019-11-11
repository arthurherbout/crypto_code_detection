#include <iostream>
#include <algorithm>
using namespace std;
int a[110][110];
bool b[110][110];
int main()
{
    freopen("in.txt","r",stdin);
    freopen("out.txt","w",stdout);
    int i,j,k,t,tt,w,h,r,rr,cc,ans;
    scanf("%d",&tt);
    for (t=1;t<=tt;t++)
    {
        scanf("%d%d%d",&h,&w,&r);
        memset(a,0,sizeof(a));
        memset(b,1,sizeof(b));
        for (i=0;i<r;i++)
        {
            scanf("%d%d",&rr,&cc);
            b[rr][cc]=0;
        }
        ans=0;
        a[1][1]=1;
        for (i=2;i<=h;i++)
            for (j=2;j<=w;j++)
                if (b[i][j])
                {
                    a[i][j]=(a[i-1][j-2]+a[i-2][j-1])%10007;
                }
        printf("Case #%d: %d\n",t,a[h][w]); 
    }
    return 0;
}