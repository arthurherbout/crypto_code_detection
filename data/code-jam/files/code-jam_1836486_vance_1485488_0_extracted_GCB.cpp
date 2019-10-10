#include<cstdio>
#include<cstring>
#include<cmath>
#include<algorithm>
#include<vector>
#include<string>
#include<queue>
#include<map>
using namespace std;
typedef long long LL;
typedef vector<int> VI;
typedef vector<double> VD;
typedef pair<int,int> PII;
#define PB push_back
#define MP make_pair
#define fi first
#define se second
#define inf 1e9
#define eps 1e-9
priority_queue<pair<int,int> >q;
int dp[200][200];
int go[4][2]={{0,1},{1,0},{0,-1},{-1,0}};
int main()
{
    int t,h,n,m,i,j,k;
    int ce[200][200],fl[200][200];
    freopen("gcb1.in","r",stdin);
    freopen("gcb.out","w",stdout);
    scanf("%d",&t);
    for(int cn=1;cn<=t;cn++)
    {
        scanf("%d %d %d",&h,&n,&m);
        //printf("%d: %d %d\n",cn,n,m);
        for(i=0;i<n;i++)for(j=0;j<m;j++)
            scanf("%d",&ce[i][j]);
        for(i=0;i<n;i++)for(j=0;j<m;j++)
            scanf("%d",&fl[i][j]);
        while(!q.empty())q.pop();
        for(i=0;i<n;i++)for(j=0;j<m;j++)dp[i][j]=inf;
        q.push(MP(0,0));
        dp[0][0]=0;
        while(!q.empty())
        {
            pair<int,int>qt=q.top();
            int cr=qt.se/m,cc=qt.se%m;
            int ct=-q.top().fi;
            q.pop();
            if(ct>dp[cr][cc])
                continue;
            //printf("%d %d %f:\n",cr,cc,ct);
            for(i=0;i<4;i++)
            {
                int nr=cr+go[i][0],nc=cc+go[i][1];
                if(nr>=0&&nr<n&&nc>=0&&nc<m)
                {
                    int hc=min(ce[cr][cc],ce[nr][nc]),hf=max(fl[cr][cc],fl[nr][nc]);
                    int hd=h-ct;
                    //printf("%d %d %d %d %f\n",nr,nc,hc,hf,hd);
                    if(hc-max(hd,hf)>=50)
                    {
                        int cost;
                        if(hd-fl[cr][cc]>=20)
                            cost=10;
                        else
                            cost=100;
                        if(ct==0)
                            cost=0;
                        if(dp[nr][nc]>ct+cost)
                        {
                            dp[nr][nc]=ct+cost;
                            q.push(MP(-(ct+cost),nr*m+nc));
                        }
                    }
                    else if(hc-hf>=50&&hd>hf)
                    {
                        int nd=hc-50;
                        int cost=hd-nd;
                        if(nd-fl[cr][cc]>=20)
                            cost+=10;
                        else
                            cost+=100;
                        if(dp[nr][nc]>ct+cost)
                        {
                            dp[nr][nc]=ct+cost;
                            q.push(MP(-(ct+cost),nr*m+nc));
                        }
                    }
                }
            }
        }
        /*for(i=0;i<n;i++)
        {
            for(j=0;j<m;j++)
                if(dp[i][j]!=inf)printf("%.1f ",dp[i][j]/10.0);
                else printf("inf ");
            printf("\n");
        }*/
        printf("Case #%d: %f\n",cn,dp[n-1][m-1]/10.0);
    }
    return 0;
}
