#include<cstring>
#include<algorithm>
#include<cstdio>
#include<vector>
#include<string>

#define MAXX 22

char buf[55];
std::string the[13][211333];
int sz[13];
int i,j,k,o;
short t,T;
int dp[55];

inline int cal(int a,int k)
{
    static int i,re,j;
    static std::vector<int>cnt;
    re=0x3f3f3f3f;
    for(i=0;i<sz[k];++i)
    {
        cnt.resize(0);
        for(j=0;j<k;++j)
            if(buf[a+j]!=the[k][i][j])
                cnt.push_back(j);
        for(j=1;j<cnt.size();++j)
            if(cnt[j]-cnt[j-1]>=5)
                break;
        if(j>=cnt.size())
            re=std::min(re,(int)cnt.size());
    }
//    printf("%d %s\n",re,the[k][o]);
    return re;
}

int main()
{
    freopen("garbled_email_dictionary.txt","r",stdin);
    while(gets(buf))
    {
        i=strlen(buf);
        the[i][sz[i]++]=buf;
    }
    freopen("c.in","r",stdin);
//    freopen("asdf","r",stdin);
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
                dp[i]=std::min(dp[i],dp[j]+cal(j+1,i-j));
        }
        printf("Case #%hd: %d\n",t,dp[i-1]);
    }
    return 0;
}
