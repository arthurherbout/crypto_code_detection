#include<cstdio>
#include<iostream>
#include<cstring>
#include<algorithm>
#include<cmath>
#include<queue>
#include<stack>
#include<map>
#include<vector>
#include<string>
#include<set>
#include<list>
#include<bitset>
#include<sstream>
#include<cstdlib>
#include<ctime>
using namespace std;
#define eps 1e-6
#define PI acos(-1)
typedef unsigned long long ll;
const int inf=0x3f3f3f3f;
//const ll linf=ceil(pow(2.0,62));
//const double dinf=pow(2.0,62);
const int maxn=105;

int T,n,m,R,C,b[maxn][maxn];
vector<int>a;
int hashmap[maxn][maxn*2];

void dfs(int cnt,int cur)
{
    if(cnt==n)
    {
        a.push_back(cur);
        return;
    }
    for(int i=2; i<=m; i++)
        dfs(cnt+1,cur*10+i);
}


int d[maxn];
void fen(int x)
{
    int cnt=0;
    while(x>0)
    {
        d[++cnt]=x%10;
        x/=10;
    }
}
void dfs2(int cnt,int cur,int id)
{
    hashmap[id][cur]=1;
    if(cnt==n+1)return;
    dfs2(cnt+1,cur*d[cnt],id);
    dfs2(cnt+1,cur,id);
}

int main()
{
    freopen("C-small-1-attempt0.in","r",stdin);
    freopen("out.out","w",stdout);
    scanf("%d%d%d%d%d",&T,&R,&n,&m,&C);
    dfs(0,0);
    //printf("%d\n",a.size());
    for(int i=1; i<=R; i++)
        for(int j=1; j<=C; j++)
            scanf("%d",&b[i][j]);
    for(int i=0; i<a.size(); i++)
    {
        fen(a[i]);
        dfs2(1,1,i);
    }
    printf("Case #1:\n");
    for(int i=1; i<=R; i++)
    {
        for(int j=0; j<a.size(); j++)
        {
            bool sign=0;
            for(int k=1; k<=C; k++)
                if(!hashmap[j][b[i][k]])
                {
                    sign=1;
                    break;
                }
            if(sign==0)
            {
                printf("%d\n",a[j]);
                break;
            }
        }
    }
    return 0;
}
