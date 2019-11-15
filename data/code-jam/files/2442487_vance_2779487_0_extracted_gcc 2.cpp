#include<cstdio>
#include<algorithm>
#include<cstring>
#include<vector>
using namespace std;
int op[3000],vst[3000];
int a[3000],b[3000];
int main()
{
    int t,n,i,j;
    freopen("cs.in","r",stdin);
    freopen("cs.out","w",stdout);
    scanf("%d",&t);
    for(int cnt=1;cnt<=t;cnt++)
    {
        scanf("%d",&n);
        for(i=0;i<n;i++)
            scanf("%d",a+i);
        for(i=0;i<n;i++)
            scanf("%d",b+i);
        memset(vst,0,sizeof(vst));
        int p = 0;
        for(i=0;i<n;i++)if(!vst[i])
        {
            int an = a[i],bn = b[i],nn=0;
            for(j=i;j<=n;j++)if(a[j]==an)
                nn++;
            p+=nn;
            int tt=p;
            for(j=i;j<=n;j++)
            {
                if(a[j]==an)
                {
                    op[j] = tt--;
                    vst[j] = 1;
                }
            }
        }
        printf("Case #%d:",cnt);
        for(i=0;i<n;i++)printf(" %d",op[i]);printf("\n");
    }
    return 0;
}
