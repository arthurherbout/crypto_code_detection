#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
using namespace std;

const int base=10007;

int n,m;
int r;
int x[10];
int y[10];

int c[base][base];
int C(int n,int m)
{
    return (c[n/10007][m/10007] * c[n%10007][m%10007])%base;
}

int Calc0(int w,int h)
{
    int x=w*2-h;
    int y=h*2-w;
    if (x<0 || y<0 || x%3!=0 || y%3!=0) return 0;
    x/=3;
    y/=3;
    return C(x+y,x);
};

map<pair<pair<int,int>,pair<int,int> >,int> M;

int Calc1(pair<int,int> s,pair<int,int> t)
{
    if (M.find(pair<pair<int,int>,pair<int,int> >(s,t))==M.end())
    {
        int ans=Calc0(t.first-s.first,t.second-s.second);
        for (int i=0;i<r;i++)
        if (x[i]>s.first && x[i]<t.first && y[i]>s.second && y[i]<t.second)
        {
            pair<int,int> mid(x[i],y[i]);
            ans=(ans-Calc0(x[i]-s.first,y[i]-s.second)*Calc1(mid,t))%base;
        }
        ans=(ans+base)%base;
        M[pair<pair<int,int>,pair<int,int> >(s,t)]=ans;
    }
    return M[pair<pair<int,int>,pair<int,int> >(s,t)];
}

int ans;

int t;
int main()
{
freopen("D.in","r",stdin);
freopen("D.out","w",stdout);

    for (int i=0;i<base;i++)
    {
        c[i][0]=c[i][i]=1;
        for (int j=1;j<i;j++) c[i][j]=(c[i-1][j]+c[i-1][j-1])%base;
    }
    
    cin>>t;
    for (int i=1;i<=t && cout<<"Case #"<<i<<": ";i++)
    {
        cin>>n>>m>>r;
        for (int i=0;i<r;i++)
        {
            cin>>x[i]>>y[i];
            x[i]--;
            y[i]--;
        }
        M.clear();
        cout<<Calc1(pair<int,int>(0,0),pair<int,int>(n-1,m-1))<<endl;
    }
    return 0;
}
