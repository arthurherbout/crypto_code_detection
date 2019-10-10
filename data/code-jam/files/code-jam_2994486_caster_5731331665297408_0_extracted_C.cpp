#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <iostream>

using namespace std;

const int MAXN = 55;
string zipCode[MAXN];

bool vis[MAXN];
string ans;
bool mp[MAXN][MAXN];
int N;

int que[MAXN],fath[MAXN];
string res[MAXN];

void search(int l,int r)
{
    if (r == N) 
    {
        fill(res,res + N,string());
        
        for(int i = N-1; i > 0; i--)
        {
            res[fath[i]] = zipCode[que[i]] + res[que[i]] + res[fath[i]]; 
            //cout << que[i] << ' ' << fath[i] << endl;
        }
        //cout << "***" << endl;
        res[que[0]] = zipCode[que[0]] + res[que[0]];
        
        if (res[que[0]] < ans) ans = res[que[0]];
    }
    else if (l < r)
    {
        //cerr << l << ' ' << r << ' ' << que[r-1] << endl;
        
        search(l + 1,r);
        for(int i = 0; i < N; i++)
        if (!vis[i] && mp[que[l]][i])
        {
            vis[i] = true;
            
            fath[r] = que[l];
            que[r] = i;
            search(l,r + 1);
            
            vis[i] = false;
        }
    }
}

int main()
{
    int tnum;
    scanf("%d",&tnum);
    for(int t = 1; t <= tnum; t++)
    {
        int n,m;
        cin >> n >> m;
        for(int i = 0; i < n; i++)
            cin >> zipCode[i];
            
        memset(mp,false,sizeof(vis));
        for(int i = 0; i < m; i++)
        {
            int u,v;
            cin >> u >> v;
            --u,--v;
            mp[u][v] = mp[v][u] = true;
        }
        
        memset(vis,false,sizeof(vis));
        
        string mz = "999999";
        int mp;
        for(int i = 0; i < n; i++)
        if (zipCode[i] < mz)
        {
            mz = zipCode[i];
            mp = i;
        }
        
        ans = "999999999999999";
        N = n;
        vis[mp] = true;
        memset(que,0,sizeof(que));
        memset(fath,0,sizeof(fath));
        que[0] = mp;
        search(0,1);
        vis[mp] = false;
        
        printf("Case #%d: ",t);
        cout << ans << endl;
    }
    return 0;
}
