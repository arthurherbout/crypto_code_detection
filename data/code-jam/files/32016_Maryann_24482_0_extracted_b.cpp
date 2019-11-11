#include<cstdio>
#include<vector>
#include<utility>
using namespace std;
int n,m,ans1,ans2;
bool f[11];
vector<pair<int,int> > d[100];
bool sat(int id)
{
    for(int i=0; i<d[id].size(); i++)
    if(d[id][i].second==f[d[id][i].first])
    return true;
    return false;
}
bool ok(int x)
{
    for(int i=1; i<=n; i++)
    {
        f[i]=x%2;
        x/=2;
    }
    for(int i=0; i<m; i++)
    if(!sat(i))
    return false;
    return true;
}
int main()
{
    int _,j,t1,t2,w;
    scanf("%d",&_);
    for(int t=1; t<=_; t++)
    {
        scanf("%d%d",&n,&m);
        for(int i=0; i<m; i++)
        {
            d[i].clear();
            for(scanf("%d",&w); w--;)
            {
                scanf("%d%d",&t1,&t2);
                d[i].push_back(make_pair(t1,t2));
            }
        }
        ans2=-1;
        for(int i=0; i<(1<<n); i++)
        if(ok(i))
        {
            t1=i,t2=0;
            for(j=0; j<n; j++)
            t2+=t1%2,t1/=2;
            if(ans2==-1 || t2<ans2)
            ans1=i,ans2=t2;
        }
        printf("Case #%d:",t);
        if(ans2==-1)
        puts(" IMPOSSIBLE");
        else
        {
            for(int i=1; i<=n; i++)
            {
                printf(" %d",ans1%2);
                ans1/=2;
            }
            puts("");
        }
    }
    return 0;
}
