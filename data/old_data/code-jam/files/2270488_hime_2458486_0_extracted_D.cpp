#include<cstdio>
#include<cstring>
#include<vector>
#include<set>

#define MAXX 211
#define OK ((1<<n)-1)

bool done[1<<21];
short t,T;
int n,m,i,j,k;
int key[MAXX],nd[MAXX];
//std::set<int>can;
std::vector<int>chst[MAXX],ans;
//int edge[MAXX],to[MAXX<<1],nxt[MAXX<<1],cnt;

bool dfs(int now)
{
    if(now==OK)
        return true;
    done[now]=true;
    int i,j;
    for(i=0;i<n;++i)
        if(key[nd[i]] && !(now&(1<<i)) && !done[now|(1<<i)])
        {
            --key[nd[i]];
            for(j=0;j<chst[i].size();++j)
                ++key[chst[i][j]];
            ans.push_back(i);
            if(dfs(now|(1<<i)))
                return true;
            ans.pop_back();
            for(j=0;j<chst[i].size();++j)
                --key[chst[i][j]];
            ++key[nd[i]];
        }
    /*
    for(std::set<int>::const_iterator it(can.begin());it!=can.end();++it)
    {
        if(!(now&(1<<*it)) && !done[now|(1<<*it)])
        {
            --key[nd[*it]];
            if(!key
        }
    }
    */
    return false;
}

int main()
{
    for(i=0;i<MAXX;++i)
        chst[i].reserve(MAXX);
    ans.reserve(MAXX);
    scanf("%hd",&T);
    for(t=1;t<=T;++t)
    {
        printf("Case #%hd: ",t);
        memset(done,0,sizeof(done));
        memset(key,0,sizeof(key));
        /*
        memset(edge,0,sizeof(edge));
        can.clear();
        cnt=0;
        */
        scanf("%d %d",&m,&n);
        while(m--)
        {
            scanf("%d",&i);
            ++key[i];
        }
        for(i=0;i<n;++i)
        {
            scanf("%d",&m);
            /*
            nxt[++cnt]=edge[m];
            edge[m]=cnt;
            to[cnt]=i;
            */
            nd[i]=m;
            scanf("%d",&m);
            chst[i].resize(m);
            for(j=0;j<m;++j)
                scanf("%d",&chst[i][j]);
        }
        /*
        for(i=0;i<MAXX;++i)
            if(key[i])
                for(j=edge[i];j;j=nxt[j])
                    can.insert(to[j]);
                    */
        ans.resize(0);
        if(dfs(0))
            for(--n,i=0;i<=n;++i)
                printf("%d%c",ans[i]+1,i<n?' ':'\n');
        else
            puts("IMPOSSIBLE");
    }
    return 0;
}
