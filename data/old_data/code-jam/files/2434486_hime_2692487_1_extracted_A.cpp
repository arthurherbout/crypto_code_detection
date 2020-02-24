#include<cstdio>
#include<cstring>
#include<algorithm>

#define MAXX 2000111
#define inf 0x3f3f3f3f3f3f3f3f

short T,t;

long long a;
int n,i,j,k;
int the[MAXX];
int ans;

int main()
{
    scanf("%hd",&T);
    for(t=1;t<=T;++t)
    {
        scanf("%lld %d",&a,&n);
        for(i=0;i<n;++i)
            scanf("%d",the+i);
        printf("Case #%hd: ",t);
        std::sort(the,the+n);
        if(a>the[n-1])
        {
            puts("0");
            continue;
        }
        if(a==1ll)
        {
            printf("%d\n",n);
            continue;
        }
        ans=n;
        for(j=i=0;i<n;++i)
        {
            while(a<=the[i])
            {
                a+=a-1;
                ++j;
            }
            a+=the[i];
            ans=std::min(ans,j+n-i-1);
        }
        printf("%d\n",ans);
    }
    return 0;
}
