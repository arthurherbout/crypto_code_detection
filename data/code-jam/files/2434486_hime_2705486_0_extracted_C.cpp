#include<cstring>
#include<algorithm>
#include<cstdio>

#define MAXX 22

char buf[55];
int sz[13];
char the[13][211253][55];
int i,j,k;
short t,T;
int dp[55];

inline int cal(int a,int b)
{
    static int i,re,j,t,l,k;
    k=b-a+1;
    re=0x3f3f3f3f;
    for(i=0;i<sz[k];++i)
    {
        for(j=0;the[k][i][j];++j)
            if(buf[a+j]!=the[k][i][j])
                break;
        if(the[k][i][j])
            l=j;
        else
            return 0;
        for(t=1,++j;the[k][i][j];++j)
            if(buf[a+j]!=the[k][i][j])
            {
                if(j-l<5)
                    break;
                ++t;
                l=j;
            }
        if(the[k][i][j])
            continue;
        re=std::min(re,t);
    }
    return re;
}

int main()
{
    freopen("garbled_email_dictionary.txt","r",stdin);
    while(gets(buf))
    {
        i=strlen(buf);
        memcpy(the[i][sz[i]++],buf,sizeof(buf));
    }
    freopen("c.in","r",stdin);
    scanf("%hd",&T);
    for(t=1;t<=T;++t)
    {
        scanf("%s",buf+1);
        memset(dp,0x3f,sizeof(dp));
        dp[0]=0;
        for(i=1;buf[i];++i)
        {
            k=std::max(0,i-10);
            for(j=i-1;j>=k;--j)
                dp[i]=std::min(dp[i],dp[j]+cal(j+1,i));
        }
        printf("Case #%hd: %d\n",t,dp[i-1]);
    }
    return 0;
}
