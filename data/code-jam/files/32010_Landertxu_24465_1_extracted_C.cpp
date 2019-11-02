#include <iostream>
#include <vector>

#define mod 1000000009

using namespace std;

typedef long long ll;

ll k;
int n;
vector <vector <int> > g;


ll perm (ll a, ll b)
{
    if (a<b)
        return 0;
    if (a<0 or b<0)
        return 0;
    ll resp=a;
    ll c=a-b;
    a--;
    while (a>c)
    {
        resp*=a;
        resp%=mod;
        a--;
    }
    return resp;
}

ll calc (int a, int p, int us, int gen)
{
    int nh=g[a].size();
    if (gen>0)
        nh--;
    ll lib=k-us;
    if (gen>1)
        lib--;
    ll resp=1;
    if (nh>0) resp=perm (lib,nh);
    for (int i=0;i<g[a].size();i++)
    {
        int b=g[a][i];
        if (b==p)
            continue;
        resp*=calc(b,a,nh,gen+1);
        resp%=mod;
    }
    return resp%mod;
}

int main()
{
    int C;
    cin >> C;
    for (int caso=1;caso<=C;caso++)
    {
        cin >> n >> k;
        g=vector <vector <int> > (n);
        for (int i=1;i<n;i++)
        {
            int a,b;
            cin >> a >> b;
            a--;b--;
            g[a].push_back(b);
            g[b].push_back(a);
        }
        cout << "Case #" << caso << ": " << calc(0,-1,0,0) << endl;
    }
}
