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

        int answer = numeric_limits<int>::max();

        for (int min_value = -1000000; min_value <= 1000000; ++min_value) {
            int have = 0;
            int maxs = min_value - mint[0] + maxt[0];
            for (int i = 0; i < K; ++i) {
                have += min_value - mint[i];
                maxs = max(maxs, min_value - mint[i] + maxt[i]);
            }

            if (have > orig)
                break;

            int grow = orig - have;
            for (int i = 0; i < K; ++i)
                grow -= maxs - (min_value - mint[i] + maxt[i]);

            if (grow > 0) {
                int times = grow / K;
                if (grow % K)
                    ++times;
                maxs += times;
            }
            answer = min(answer, maxs - min_value);
        }

        cout << "Case #" << test << ": " << answer << "\n";
    }
}
