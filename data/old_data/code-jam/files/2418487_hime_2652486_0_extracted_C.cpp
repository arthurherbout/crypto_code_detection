#include<cstdio>
#include<cstring>
#include<map>

#define MAXX 1111

short T;
int r,n,m,k,i,j,tval,cnt,lim,sum,K;
int rs[MAXX];
int ans[13];
bool done[MAXX];
std::map<int,int>map;
std::map<int,int>::const_iterator it;

bool dfs(int now,int pre)
{
    if(now==n)
    {
        memset(done,0,sizeof(done));
        cnt=0;
        for(i=1;i<=lim;++i)
        {
            tval=1;
            for(j=0;j<n;++j)
                if(i&(1<<j))
                    tval*=ans[j];
            if(!done[tval])
            {
                done[tval]=true;
                cnt+=map[tval];
            }
        }
        return cnt>=K;
        /*
        for(i=0;i<n;++i)
            printf("%d ",ans[i]);
        puts("");
        puts("");
        printf("cnt %d\n",cnt);
        return cnt==k;
        */
    }
    for(ans[now]=pre;ans[now]<=m;++ans[now])
        if(dfs(now+1,ans[now]))
            return true;
    return false;
}

int main()
{
    puts("Case #1:");
    scanf("%hd",&T);
    scanf("%d %d %d %d",&r,&n,&m,&k);
    lim=(1<<n)-1;
    sum=0;
    while(r--)
    {
        map.clear();
        for(i=j=0;i<k;++i)
        {
            scanf("%d",rs+i);
            if(rs[i]!=1)
                ++map[rs[i]];
            else
                ++j;
        }
        K=k-j;
        if(dfs(0,2))
            for(i=0;i<n;++i)
                printf("%d",ans[i]);
        else
            for(i=0;i<n;++i)
                putchar('2');
        puts("");
    }
//    printf("%d\n",sum);
    return 0;
}
