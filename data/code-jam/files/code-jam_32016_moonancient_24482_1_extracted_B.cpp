#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;
#define MAXN 2010
#define MAXM 2010

int n;
int m;
vector<int> q0[MAXM];
int q1[MAXM];

int ans[MAXN];

bool Satisfied(int u)
{
    if (q1[u]!=-1 && ans[q1[u]]) return true;
    for (int i=0;i<q0[u].size();i++)
    if (!ans[q0[u][i]]) return true;
    return false;
}

int t;
int main()
{
freopen("B.in","r",stdin);
freopen("B.out","w",stdout);
    cin>>t;
    for (int i=1;i<=t && cout<<"Case #"<<i<<":";i++)
    {
        int i,j,k;
        cin>>n>>m;
        for (i=0;i<m;i++)
        {
            cin>>k;
            q0[i].clear();
            q1[i]=-1;
            while (k--)
            {
                int x,y;
                cin>>x>>y;
                x--;
                if (y==0) q0[i].push_back(x);
                else q1[i]=x;
            }
        }
        memset(ans,0,sizeof(ans));
        for (i=0;i<m;i++)
        if (!Satisfied(i))
        {
            if (q1[i]!=-1)
            {
                ans[q1[i]]=1;
                i=-1;
            }
            else break;
        }
        if (i==m)
        {
            for (i=0;i<n;i++) cout<<' '<<ans[i];
            cout<<endl;
        }
        else puts(" IMPOSSIBLE");
    }
    return 0;
}
