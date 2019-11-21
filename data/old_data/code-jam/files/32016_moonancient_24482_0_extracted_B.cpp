#include <iostream>
#include <algorithm>
using namespace std;
#define MAXN 2010
#define MAXM 2010

int n;
int m;
int q0[MAXM];
int q1[MAXM];

bool Check(int s)
{
    int mask=(1<<n)-1;
    for (int i=0;i<m;i++)
    {
        if (!(q0[i] & (s ^ mask)) && !(q1[i] & s)) return false;
    }
    return true;
}

int Count(int s)
{
    int ans=0;
    while (s)
    {
        ans+=s&1;
        s>>=1;
    }
    return ans;
}

int ans;

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
            q0[i]=q1[i]=0;
            while (k--)
            {
                int x,y;
                cin>>x>>y;
                x--;
                if (y==0) q0[i]|=1<<x;
                else q1[i]|=1<<x;
            }
        }
        ans=-1;
        for (k=0;k<(1<<n);k++)
        if (Check(k))
        {
            if (ans==-1 || Count(k)<Count(ans)) ans=k;
        }
        if (ans!=-1)
        {
            for (i=0;i<n;i++) cout<<' '<<!!(ans & (1<<i));
            cout<<endl;
        }
        else puts(" IMPOSSIBLE");
    }
    return 0;
}
