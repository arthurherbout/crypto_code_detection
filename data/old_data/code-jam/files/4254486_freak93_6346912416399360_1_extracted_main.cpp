#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    int T; cin >> T;

    for (int test = 1; test <= T; ++test) {
        int N, K; cin >> N >> K;
        vector<int> diff(N, 0);
        vector<int> maxt(K, 0), mint(K, 0);
        int sum0; cin >> sum0;
        int orig = sum0;
        for (int i = K; i < N; ++i) {
            int sum; cin >> sum;
            diff[i] = sum - sum0 + diff[i - K];
            maxt[i % K] = max(maxt[i % K], diff[i]);
            mint[i % K] = min(mint[i % K], diff[i]);
            sum0 = sum;
        }

        int64_t answer = numeric_limits<int64_t>::max();
        int maxdiff = 0;
        for (int i = 0; i < K; ++i)
            maxdiff = max(maxdiff, maxt[i] - mint[i]);
        int64_t summaxt = 0;
        for (int i = 0; i < K; ++i)
            summaxt += maxt[i];
        int64_t start = orig - 1LL * maxdiff * K + summaxt;

        for (int64_t min_value = start / K - 3; ; ++min_value) {
            int64_t have = 0;
            int64_t maxs = min_value - mint[0] + maxt[0];
            for (int i = 0; i < K; ++i) {
                have += min_value - mint[i];
                maxs = max(maxs, min_value - mint[i] + maxt[i]);
            }

            if (have > orig)
                break;

            int64_t grow = orig - have;
            for (int i = 0; i < K; ++i)
                grow -= maxs - (min_value - mint[i] + maxt[i]);

            if (grow > 0) {
                int64_t times = grow / K;
                if (grow % K)
                    ++times;
                maxs += times;
            }
            answer = min(answer, maxs - min_value);
        }

        cout << "Case #" << test << ": " << answer << "\n";
    }
}
