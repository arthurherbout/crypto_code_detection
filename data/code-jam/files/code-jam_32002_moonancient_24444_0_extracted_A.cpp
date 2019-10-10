#include <iostream>
#include <algorithm>
using namespace std;
#define MAXX 300

const int dir[4][2]=
{
    0,1,
    1,0,
    0,-1,
    -1,0
};

int n;
string s;
int m;
bool adj[MAXX][MAXX][4];
bool flag[MAXX][MAXX];

void DFS(int x,int y)
{
    if (flag[x][y]) return;
    flag[x][y]=true;
    for (int i=0;i<4;i++)
    if (adj[x][y][i]) DFS(x+dir[i][0],y+dir[i][1]);
}

void Draw(int x,int y,int d)
{
    if (d==2) Draw(x,y-1,0);
    if (d==3) Draw(x-1,y,1);
    if (d==0)
    {
        adj[x-1][y][1]=false;
        adj[x][y][3]=false;
    }
    if (d==1)
    {
        adj[x][y-1][0]=false;
        adj[x][y][2]=false;
    }
}

int ans;

int t;
int main()
{
freopen("A.in","r",stdin);
freopen("A.out","w",stdout);

    cin>>t;
    for (int i=1;i<=t && cout<<"Case #"<<i<<": ";i++)
    {
        int x,y,d;
        int i,j,k;
        cin>>n;
        memset(adj,true,sizeof(adj));
        x=150;
        y=150;
        d=0;
        while (n--)
        {
            cin>>s>>m;
            while (m--)
            {
                for (int i=0;i<s.size();i++)
                {
                    if (s[i]=='L') d=(d+3)%4;
                    else if (s[i]=='R') d=(d+1)%4;
                    else
                    {
                        Draw(x,y,d);
                        x+=dir[d][0];
                        y+=dir[d][1];
                    }
                }
            }
        }
        
        for (i=0;i<MAXX;i++)
        {
            adj[0][i][3]=false;
            adj[i][0][2]=false;
            adj[MAXX-1][i][1]=false;
            adj[i][MAXX-1][0]=false;
        }
        
        memset(flag,false,sizeof(flag));
        DFS(0,0);
        
        ans=0;
        for (i=0;i<MAXX;i++)
        for (j=0;j<MAXX;j++)
        if (flag[i][j])
        {
            bool crash[4];
            memset(crash,false,sizeof(crash));
            if (i==150 && j==150)
            {
                i=150;
            }
            for (k=0;k<4;k++)
            {
                x=i;
                y=j;
                while (adj[x][y][k])
                {
                    x+=dir[k][0];
                    y+=dir[k][1];
                }
                if (x!=0 && x!=MAXX-1 && y!=0 && y!=MAXX-1) crash[k]=true;
            }
            if ((crash[0] && crash[2]) || (crash[1] && crash[3]))
            {
                ans++;
            }
        }
        
        cout<<ans<<endl;
    }
    return 0;
}
