#include <iostream>
#include <map>
#include <string>
#include <iomanip>
#include <cstring>

using namespace std;

int len(int mask) {
    int ret = 0;
    while (mask) {
        ret += mask & 1;
        mask >>= 1;
    }
    return ret;
}

void solve(int e) {
    cout << "Case #" << e << ": ";

    int n, k; cin >> n >> k;
    long double p[202]; for (int i=0; i<n; ++i) cin >> p[i];

    long double mx = 0;
    for (int mask=3; mask < (1 << n); ++mask) {
        if (len(mask) == k) {
            long double dp[202][202] = {};

            int cur = 1;
            int prv = 0;

            dp[prv][101] = 1.0;

            for (int i=0; i<n; ++i) {
                if (mask & (1 << i)) {
                    memset(dp[cur], 0, sizeof dp[cur]);
                    // take p[i]
                    for (int u=0; u<=200; ++u) {
                        dp[cur][u+1] += p[i] * dp[prv][u];
                        dp[cur][u-1] += (1.0 - p[i]) * dp[prv][u];
                    }
                    cur ^= 1;
                    prv ^= 1;
                }
            }

            if (dp[prv][101] > mx) {
                mx = dp[prv][101];
            }
        }
    }

    cout << fixed << setprecision(9) << mx << endl;
}

int main() {
    int t; cin >> t;
    for (int e=1; e<=t; ++e)
        solve(e);
}
