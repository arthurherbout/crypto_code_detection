#include<algorithm>
#include<iostream>
#include<cstdlib>
#include<cstdio>
#include<cmath>
#include<queue>

#define ll long long
#define mp make_pair
#define x first
#define y second
#define all(a) (a).begin(), (a).end()
using namespace std;

const int N=50*50;

vector <int> a;
string res, ans, mn;
int q, w, j, i, color[N], m, n, used[N], num, t, c, g[N][N];

priority_queue < pair<int, int> > A;

string conv(int x)
{
    vector <int> a;
    string r="";
    int i;
    while(x>0)
    {
        r=r+char(x%10+'0');
        x/=10;
    }
    reverse(all(r));
    return r;
}

void dfs(int v)
{
    used[v]=1;
    for(int i=1; i<=n; i++)
        if(g[v][i]&&used[i]==0)
        {
            A.push(mp(-color[i], i));
            used[i]=1;
        }
    if(!A.empty())
    {
        pair <int, int> x=A.top();
        x.x=-x.x;
        res+=conv(x.x);
        A.pop();
        dfs(x.y);
    }
}

int main()
{
    cin.tie(0);
    ios_base::sync_with_stdio(0);
    freopen("C-small-attempt0.in","r",stdin);
    freopen("1.txt","w",stdout);
    cin>>t;
    while(t--)
    {
        num++;
        ans="z";
        cin>>n>>m;
        for(i=1; i<=n; i++)
            cin>>color[i];
        for(i=1; i<=m; i++)
        {
            cin>>q>>w;
            g[q][w]=1;
            g[w][q]=1;
        }
        mn="z";
        for(i=1; i<=n; i++)
            if(conv(color[i])<mn)
            {
                mn=conv(color[i]);
                a.clear();
                a.push_back(i);
            } else
            if(conv(color[i])==mn)a.push_back(i);
        for(i=0; i<a.size(); i++)
        {
            for(j=1; j<=n; j++)
                used[j]=0;
            res=conv(color[a[i]]);
            dfs(a[i]);
            ans=min(ans, res);
        }
        cout<<ans<<endl;
    }
}
