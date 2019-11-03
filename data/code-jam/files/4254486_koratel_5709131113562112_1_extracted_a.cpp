#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <queue>
#include <algorithm>
#include <map>
#include <cmath>
#include <set>
#include <iomanip>

using namespace std;

typedef long long ll;
typedef vector<int> vi;
typedef pair<int, int> pii;

#define fi first
#define se second
#define mp make_pair
#define pb push_back

const int INF = 1 << 30;
const double EPS = 1e-8;

vector<vector<int> > v;
vector<int> conn, dead;
int cur=0, ans=1;

void go(int x)
{
    //cout<<"go "<<x<<endl;
    if(dead[x]) return;
    queue<int> q;
    q.push(x);
    while(q.size())
    {
        x=q.front();
        q.pop();
        cur++;
        conn[x]=1;
        for(int i=0;i<(int)v[x].size();i++)
        {
            int next=v[x][i];
            if(!dead[next]) q.push(next);
        }
    }
}

void boom(int x)
{
    //cout<<"boom "<<x<<endl;
    if(dead[x]) return;
    queue<int> q;
    q.push(x);
    while(q.size())
    {
        x=q.front();
        q.pop();
        cur-=conn[x];
        conn[x]=0;
        dead[x]=1;
        for(int i=0;i<(int)v[x].size();i++)
        {
            int next=v[x][i];
            if(!dead[next]) q.push(next);
        }
    }
}

void solve(int num)
{
    int n, d;
    cin>>n>>d;
    ll s0,as,cs,rs,m0,am,cm,rm;
    cin>>s0>>as>>cs>>rs>>m0>>am>>cm>>rm;
    vector<ll> s(n),m(n);
    v.clear();
    v.resize(n);
    conn.clear();
    conn.resize(n,0);
    conn[0]=1;
    dead.clear();
    dead.resize(n,0);
    s[0]=s0; m[0]=m0;
    vector<vector<int> > sal(rs);
    for(int i=1;i<n;i++)
    {
        s[i]=(s[i-1]*as+cs)%rs;
        sal[s[i]].pb(i);
        m[i]=(m[i-1]*am+cm)%rm;
    }
    for(int i=1;i<n;i++) m[i]%=i;

    //cout<<"wut?"<<endl;

    int low=max(0LL,s[0]-d), hi=min((ll)low+d,rs-1);
    for(int i=low;i<=hi;i++)
    {
        for(int j=0;j<(int)sal[i].size();j++)
        {
            int x=sal[i][j];
            v[m[x]].pb(x);
        }
    }
    //cout<<"wut?"<<endl;
    ans=1;
    cur=0;
    go(0);
    ans=cur;
    //cout<<"no 0 przeszlo "<<low<<" "<<hi<<endl;

    int hh=min(s[0]+d,(ll)rs-1);
    //cout<<"?? "<<hh<<endl;
    for(int i=hi+1;i<=hh;i++)
    {
        for(int j=0;j<(int)sal[i-d-1].size();j++)
        {
            int x=sal[i-d-1][j];
            boom(x);
        }
        //cout<<"bleah"<<endl;
        for(int j=0;j<(int)sal[i].size();j++)
        {
            int x=sal[i][j];
            v[m[x]].pb(x);
            if(conn[m[x]]) go(x);
        }
        //cout<<"no "<<cur<<endl;
        ans=max(ans,cur);
    }

    cout<<"Case #"<<num<<": "<<ans<<"\n";

}

int main()
{
    ios_base::sync_with_stdio(0);
    int t;
    cin>>t;
    for(int i=1; i<=t; i++)
    {
        solve(i);
    }
}

