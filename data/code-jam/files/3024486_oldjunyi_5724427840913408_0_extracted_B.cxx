#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
using namespace std;

int h[105],die[105];
int g[105],use[105],all[105];
int ans,dp[105][2][20050];

void relax(int& x, int y){
    if(x<y) x=y;
    ans=max(x,ans);
}

int main(){
    int cs,no=0;
    scanf("%d",&cs);
    while(cs--){
        int p,q,n;
        scanf("%d%d%d",&p,&q,&n);
        memset(die,0,sizeof(die));
        memset(use,0,sizeof(use));
        memset(all,0,sizeof(all));
        for(int i=0;i<n;i++){
            scanf("%d%d",h+i,g+i);
            die[i]=(h[i]+q-1)/q;
            use[i]=(h[i]-1)%q/p+1;
            all[i]=(h[i]+p-1)/p;
        }
        memset(dp,192,sizeof(dp));
        ans=dp[0][0][0]=0;
        for(int i=0;i<n;i++) for(int j=0;j<2;j++)
        for(int t=0;t<20050;t++) if(dp[i][j][t]>=0){
            relax(dp[i+1][0][t+die[i]-j],dp[i][j][t]);
            if(t>=all[i]) relax(dp[i+1][j][t-all[i]],dp[i][j][t]+g[i]);
            if(j && h[i]<=q) continue;
            int p=die[i]-j+t;
            if(p>=use[i]) relax(dp[i+1][1][p-use[i]],dp[i][j][t]+g[i]);
        }
        printf("Case #%d: %d\n",++no,ans);
    }
}
