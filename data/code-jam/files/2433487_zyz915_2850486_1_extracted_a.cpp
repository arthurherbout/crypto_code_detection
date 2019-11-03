#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>

#include <map>
#include <vector>

using namespace std;

int T, cas;

void run() {
    long long m;
    int n;
    long long a[37] = {0};
    cin >> m >> n;
    for (int i = 0; i < n; i++)
        cin >> a[i];
    sort(a, a + 37);
    double ans = 0;
    for (int k = 1; k <= 36; k++) {
        long long lo = -1, hi = 1ll<<51;
        long long g_pay = 0, g_extra = 0;
        while (lo + 1 < hi) {
            long long mid = (lo + hi) >> 1;
            long long pay = 0, extra = 0;
            for (int i = 0; i < k; i++)
                if (a[i] <= mid)
                    pay += mid - a[i];
            for (int i = k; i < 37; i++)
                if (a[i] <= mid)
                    extra += mid + 1 - a[i];
            if (pay + extra > m)
                hi = mid;
            else {
                lo = mid;
                g_pay = pay;
                g_extra = extra;
            }
        }
        //cout << lo << " " << hi << " " << g_pay << " " << g_extra << endl;
        ans = max(ans, g_pay * (36.0 - k) / k - g_extra);
    }
    cout.precision(8);
    cout << fixed;
    cout << "Case #" << cas << ": " << ans << endl;
}

int main() {
    cin >> T;
    for (cas = 1; cas <= T; cas++)
        run();
    return 0;
}
