#include<iostream>
#include<cstring>
#include<cstdio>
#include<algorithm>
#include<cmath>
#include<stack>
#include<queue>
#include<vector>
#include<map>
#include<ctime>
#include<set>
#include<string>
using namespace std;
#define ll long long
int vis[2005],num[4005],n,m,K,need[2005];
vector<int>have[2005];
int res[2005],pos[2005],a[405],b[405];
bool check()
{
    memset(a,0,sizeof(a));
    memset(b,0,sizeof(b));
    vector<int>q;
    for(int i=1;i<=n;i++)
    {
        if(!vis[i])
        {
            q.push_back(i);
            b[need[i]]++;
            for(int j=0;j<have[i].size();j++)
                if(have[i][j]==need[i]&&(j==0||have[i][j]!=have[i][j-1]))
                    continue;
                else
                    a[have[i][j]]++;
        }
    }
    for(int i = 0;i<q.size();i++)
    {
        if(a[need[q[i]]]+num[need[q[i]]] < b[need[i]])
            return false;
    }
    return true;
}
bool dfs(int idx)
{
    if(idx>n)
    {
        for(int i=1;i<=n;i++)
            res[i]=pos[i];
        return true;
    }
    if(!check())
        return false;
    //cout<<"idx="<<idx<<endl;
    for(int i=1;i<=n;i++)
    {
        if(!vis[i]&&num[need[i]]>0)
        {
            vis[i]=1;
            pos[idx]=i;
            num[need[i]]--;
            for(int j=0;j<have[i].size();j++)
            {
                num[have[i][j]]++;
            }
            if(dfs(idx+1))
                return true;
            for(int j=0;j<have[i].size();j++)
            {
                num[have[i][j]]--;
            }
            num[need[i]]++;
            vis[i]=0;
        }
    }
    return false;
}
int main()
{
    int i,j,k,T;
    freopen("d-small.in","r",stdin);
    freopen("4.txt","w",stdout);
    scanf("%d",&T);
    for(int cas=1;cas<=T;cas++)
    {
        scanf("%d%d",&K,&n);
        //if(cas==4) cout<<K<<" "<<n<<endl;
        memset(num,0,sizeof(num));
        memset(vis,0,sizeof(vis));
        for(i=1;i<=n;i++)
            have[i].clear();
        while(K--)
        {
            scanf("%d",&i);
            //if(cas==4)cout<<i<<" ";
            num[i]++;
        }
        //if(cas==4)cout<<endl;
        for(i=1;i<=n;i++)
        {
            scanf("%d%d",&need[i],&k);
            //if(cas==4)cout<<need[i]<<" "<<k;
            while(k--)
            {
                scanf("%d",&j);
                //if(cas==4)cout<<" "<<j;
                have[i].push_back(j);
            }
            sort(have[i].begin(),have[i].end());
            //if(cas==4)cout<<endl;
        }
        printf("Case #%d: ",cas);
        if(!dfs(1))
            puts("IMPOSSIBLE");
        else
        {
            for(i=1;i<=n;i++)
                printf(" %d",res[i]);
            puts("");
        }
    }
    return 0;
}
