#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

int main() {
    int T; cin >> T;

    for (int test = 1; test <= T; ++test) {
        string S; cin >> S;
        int state = 0;
        int N = S.size();

        for (int i = 0; i < N; ++i)
            if (S[i] == 'X')
                state += (1 << i);

        vector<double> dp(1 << N, 0);
        vector<double> prob(1 << N, 0);
        prob[state] = 1;

        for (int i = state; i < (1 << N); ++i) {
            if ((i & state) != state)
                continue;
            if (i == (1 << N) - 1)
                continue;

            for (int j = 0; j < N; ++j) {
                int k;
                for (k = 0; (1 << ((j + k) % N)) & i; ++k);
                prob[i + (1 << ((j + k) % N))] += prob[i] / double(N);
            }
        }

        for (int i = state; i < (1 << N); ++i) {
            if ((i & state) != state)
                continue;
            if (i == (1 << N) - 1)
                continue;
            int bits = 0;
            for (int j = 0; j < N; ++j)
                if ((1 << j) & i)
                    ++bits;
            for (int j = 0; j < N; ++j) {
                int k;
                for (k = 0; (1 << ((j + k) % N)) & i; ++k);
                int next = i + (1 << ((j + k) % N));
                dp[next] += (dp[i] + double(N - k)) * prob[i] / double(prob[next]) / double(N);
            }
        }

        cout.setf(ios::fixed, ios::floatfield);
        cout.precision(15);
        cout << "Case #" << test << ": ";
        cout << dp[(1 << N) - 1] << "\n";
    }
}
