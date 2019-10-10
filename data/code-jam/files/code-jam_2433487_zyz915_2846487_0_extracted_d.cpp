#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>

#include <map>
#include <vector>

using namespace std;

int T, cas;

double dp[1<<20], sum[1<<20];

void run() {
    char s[25];
    cin >> s;
    int n = strlen(s);
    //reverse(s, s + n);
    int mask = 0;
    for (int i = 0; i < n; i++)
        if (s[i] == 'X')
            mask ^= (1 << i);
    memset(dp, 0, sizeof(dp));
    memset(sum, 0, sizeof(sum));
    dp[mask] = 1.0;
    double ans = 0;
    for (int i = mask; i < (1 << n) - 1; i++) {
        double c = 1.0 / n;
        long long q = i + ((long long)i << n);
        for (int st = 0; st < n; st++)
        {
            int price = n, nmask = i;
            for (int j = 0; j < n; j++)
                if ((q & (1ll << st + j))) {
                    price -= 1;
                } else {
                    nmask += (1 << (st + j) % n);
                    break;
                }
            sum[i] += price * c;
            dp[nmask] += dp[i] * c;
        }
        ans += dp[i] * sum[i];
    }
    cout.precision(12);
    cout<<fixed;
    cout << "Case #" << cas << ": " << ans << endl;
}

int main() {
    cin >> T;
    for (cas = 1; cas <= T; cas++)
        run();
    return 0;
}
