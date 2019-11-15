#include<stdio.h>
#include<string.h>
typedef long long ll;
ll a[110];
inline ll min(ll a,ll b){return a<b?a:b;}
int main()
{
    //freopen("D-small-attempt9.in","r",stdin);
    //freopen("D-small-attempt9.out","w",stdout);
    ll i,j;
    int k,c,s;
    int t,cases=0;
    scanf("%d",&t);
    while(t--)
    {
        scanf("%d%d%d",&k,&c,&s);
        printf("Case #%d:",++cases);
        if(c==1)
        {
            for(i=1;i<=k;i++) printf(" %d",i);
            printf("\n");
            continue;
        }
        if(k==1)
        {
            printf(" 1\n");continue;
        }
        a[0]=1;a[1]=k;
        for(i=2;i<=c;i++)
            a[i]=a[i-1]*(ll)k;
        for(i=1;i<=k;)
        {
            ll tmp=0;
            for(j=-1;j<=c-2;j++)
            {
                if(i+j+1<=k)
                    tmp+=(i+j)*a[c-2-j];
                else
                    tmp+=(k-1)*a[c-2-j];
            }
            printf(" %I64d",tmp+1);
            i+=min(c,k-i+1);
        }
        printf("\n");
    }

    return 0;
}
