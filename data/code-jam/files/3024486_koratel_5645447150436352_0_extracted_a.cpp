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

void solve(int num)
{
    ll n, p, q, r, s, total=0;
    cin>>n>>p>>q>>r>>s;
    vector<ll> v(n+1), pref(n+1, 0);
    for(int i=1; i<=n; i++)
    {
        v[i]=(p*((ll)i-1)+q)%r+s;
        pref[i]=pref[i-1]+v[i];
        total+=v[i];
    }
    ll solvmax=total;
    for(int a=1; a<=n; a++)
    {
        ll pocz=pref[a-1], rest=total-pocz;
        int st=a-1, kon=n;
        while(st+1<kon)
        {
            int shot=(st+kon)/2;
            ll temp=pref[shot]-pref[a-1];
            if(temp*2LL>=rest) kon=shot;
            else st=shot;
        }
        for(int b=kon-4; b<kon+5; b++)
        {
            if(b<a || b>n) continue;
            ll mid=pref[b]-pref[a-1], suf=pref[n]-pref[b];
            solvmax=min(solvmax, max(pocz, max(mid,suf)));
        }
    }
    double totald=total, solvmaxd=solvmax;
    cout<<"Case #"<<num<<": "<<(total-solvmaxd)/totald<<"\n";

}

int main()
{
    ios_base::sync_with_stdio(0);
    int t;
    cin>>t;
    cout<<setprecision(12)<<fixed;
    for(int i=1; i<=t; i++)
    {
        solve(i);
    }
}

