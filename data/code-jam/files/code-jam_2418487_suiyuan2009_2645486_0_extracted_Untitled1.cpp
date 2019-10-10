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

int T,E,n,R,v[maxn];
int f[maxn][maxn];

int main()
{
    freopen("B-small-attempt0.in","r",stdin);
   freopen("out.out","w",stdout);
    int ncase=0;
    scanf("%d",&T);
    while(T--)
    {
        ncase++;
        scanf("%d%d%d",&E,&R,&n);
        memset(f,-1,sizeof(f));
        for(int i=1; i<=n; i++)scanf("%d",&v[i]);
        int m=E;
        f[0][E]=0;
        for(int i=1; i<=n; i++)
            for(int j=0; j<=m+R; j++)
            {
                int tj=min(m,j);
                if(j<R)continue;
                for(int k=0; k<=m; k++)
                {
                    if(k-(j-R)<0)continue;
                    if(f[i-1][k]==-1)continue;
                    f[i][tj]=max(f[i][tj],f[i-1][k]+(k-j+R)*v[i]);
                }
            }
        int ans=0;
        for(int i=0; i<=m; i++)ans=max(ans,f[n][i]);
        printf("Case #%d: %d\n",ncase,ans);
    }
    return 0;
}
