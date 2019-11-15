#include<algorithm>
#include<iostream>
#include<cstdlib>
#include<cstdio>
#include<vector>
#include<cmath>
#include<set>
#include<map>
using namespace std;
#define x first
#define y second
#define mp make_pair
#define ll long long
#define pb push_back
#define bro pair<int, int>
#define all(a) (a).begin(), (a).end()

const int N=1001;

bool used[N][N];
int f, c, x, j, i, T, t, k, mask, ans, a[N][N], n, m;

bool go(int x, int y)
{
    if(x<1||x>n||y<1||y>m||used[x][y]||a[x][y])return false;
    return true;
}

void dfs(int x, int y)
{
    used[x][y]=1;
    if(go(x, y+1))dfs(x, y+1);
    if(go(x, y-1))dfs(x, y-1);
    if(go(x+1, y))dfs(x+1, y);
    if(go(x-1, y))dfs(x-1, y);
}

int main()
{
    cin.tie(0);
    ios_base::sync_with_stdio(0);
    freopen("C-large.in","r",stdin);
    freopen("c.txt","w",stdout);
    cin>>T;
    while(T--)
    {
        t++;
        cin>>n>>m>>k;
        ans=k;
        for(mask=0; mask<(1<<(10)); mask++)
        {
            f=0;
            c=0;
            x=mask;
            for(i=1; i<=n; i++)
                for(j=1; j<=m; j++)
                {
                    used[i][j]=0;
                    a[i][j]=x%2;
                    c+=x%2;
                    x/=2;
                }
            if(c>=ans)continue;
            for(i=1; i<=m; i++)
            {
                if(go(1, i))dfs(1, i);
                if(go(n, i))dfs(n, i);
            }
            for(i=1; i<=n; i++)
            {
                if(go(i, 1))dfs(i, 1);
                if(go(i, m))dfs(i, m);
            }
            for(i=1; i<=n; i++)
                for(j=1; j<=m; j++)
                    if(!used[i][j])f++;
            if(f>=k)ans=min(ans, c);
        }
        cout<<"Case #"<<t<<": "<<ans<<endl;
    }
}
