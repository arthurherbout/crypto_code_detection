#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cstring>

using namespace std;

const int inf = 10000000000000LL;

double dp[1010];
int v[1010];

int main() {
    int t, n, clk = 0;
    freopen("D-small-attempt0.txt", "r", stdin);
    freopen("outD.txt", "w", stdout);
    scanf("%d", &t);
    while (t--) {
        int i, j, k, cnt = 0;
        scanf("%d", &n);
        for (i = 1; i <= n; ++i) {
            scanf("%d", v+i);
            if(v[i] != i) cnt++;
        }
        dp[0] = 0.0;
        dp[1] = 1.0;
        for (i = 2;i <= cnt; ++i) {
            if(inf / dp[i-1] <= i)
                dp[i] = inf;
            else 
                dp[i] = i * dp[i-1];
        }
        for (i = 2; i <= cnt; ++i) {
            for (j = 1; j < i; ++j) {
                dp[i] = min(dp[i] , dp[j] + dp[i-j]);
            }
        }
        //cout << dp[2] << endl;
        printf("Case #%d: %.6f\n", ++clk, dp[cnt]);
    }
    //system("pause");
    return 0;
}
