#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;
const int N = 1111;
int dp[N][N];
int a[N];
int main(){
    int t;
    int nCase = 0;
    freopen("d:\\B-large.in","r",stdin);
    freopen("d:\\B.out","w",stdout);
    for (scanf("%d",&t);t;t--){
        int n,m;
        scanf("%d",&n);
        m = 0;
        for (int i=1;i<=n;i++){
            scanf("%d",&a[i]);
            m = max(m,a[i]);
        }
        memset(dp,0,sizeof(dp));
        for (int i=1;i<=n;i++){
            for (int j=1;j<=m;j++){
                dp[i][j] = dp[i-1][j] + (a[i]-1)/j;
            }
        }
        int ans = m;
        for (int j=1;j<=m;j++) ans = min(ans,dp[n][j]+j);
        printf("Case #%d: %d\n",++nCase,ans);
    }
    return 0;
}
