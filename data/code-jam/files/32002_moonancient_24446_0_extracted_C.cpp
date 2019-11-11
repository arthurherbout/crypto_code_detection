#include <iostream>
#include <algorithm>
using namespace std;
#define MAXN 12

int n,m;
int room[MAXN];
int f[MAXN][1<<MAXN];

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

bool Available(int s)
{
    for (int i=1;i<m;i++)
    if (s&(1<<i))
    if (s & (1<<(i-1))) return false;
    return true;
}

int t;
int ans;

int main()
{
freopen("C.in","r",stdin);
freopen("C.out","w",stdout);

    cin>>t;
    for (int i=1;i<=t && cout<<"Case #"<<i<<": ";i++)
    {
        int i,j,k;
        cin>>n>>m;
        for (i=n;i>0;i--)
        {
            room[i]=0;
            for (j=0;j<m;j++)
            {
                char ch;
                cin>>ch;
                room[i]=room[i]*2+(int)(ch=='x');
            }
        }
        memset(f,-1,sizeof(f));
        f[0][0]=0;
        for (i=1;i<=n;i++)
        for (k=0;k<(1<<m);k++)
        {
            if (k==7)
            {
                k=7;
            }
            if (!Available(k) || (k & room[i])) f[i][k]=-1;
            else
            {
                f[i][k]=0;
                for (j=0;j<(1<<m);j++)
                if (f[i-1][j]>=0)
                {
                    int now=0;
                    for (int x=0;x<m;x++)
                    if (j & (1<<x))
                    {
                        if (x) now|=(1<<(x-1));
                        if (x<m-1) now|=(1<<(x+1));
                    }
                    if ((k & now)==0) f[i][k]=max(f[i][k],f[i-1][j]);
                }
                f[i][k]+=Count(k);
            }
        }
        
        ans=0;
        for (k=0;k<(1<<m);k++) ans=max(ans,f[n][k]);
        
        cout<<ans<<endl;
    }
    return 0;
}
