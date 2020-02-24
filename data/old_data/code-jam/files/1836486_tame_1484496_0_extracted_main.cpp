#include<cstdio>
#include<cstring>
#include<cmath>
#include<set>
#include<algorithm>
using namespace std;
int s[1000],n;

int ans[2000010];


int main()
{
    freopen("4.in","r",stdin);
    freopen("4.out","w",stdout);
    int cas;
    scanf("%d",&cas);
    for(int g=1;g<=cas;g++)
    {
        scanf("%d",&n);
        memset(ans,-1,sizeof(ans));
        int a=-1,b=-1,flag=0;
        for(int i=1;i<=n;i++)
            scanf("%d",&s[i]);
        int end=(1<<n)-1;
        for(int i=1;i<=end;i++)
        {
            int ss=0,t=i;
            for(int j=1;t&&j<=n;j++)
            {
                if(t%2)
                ss+=s[j];
                t/=2;
            }
            if(ans[ss]>=0)
            {
                flag=1;
                a=ans[ss];
                b=i;
                break;
            }
            ans[ss]=i;

        }
        printf("Case #%d:\n",g);
        if(!flag)
            printf("Impossible\n");
        else
        {
            int t=a;
             for(int j=1;t&&j<=n;j++)
            {
                 if(t%2)
                     printf("%d ",s[j]);
                t/=2;
            }
            printf("\n");

              t=b;
             for(int j=1;t&&j<=n;j++)
            {
                 if(t%2)
                     printf("%d ",s[j]);
                t/=2;
            }
            printf("\n");
        }

      
    }
    return 0;
}