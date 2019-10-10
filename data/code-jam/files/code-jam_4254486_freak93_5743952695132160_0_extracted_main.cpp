#include <iostream>
#include <fstream>
#include <vector>
#include <map>

using namespace std;

int main() {
    int T; cin >> T;

    for (int test = 1; test <= T; ++test) {
        int P; cin >> P;

        vector<int> E(P), F(P);
        for (int i = 0; i < P; ++i)
            cin >> E[i];

        map<int, int> M;
        for (int i = 0; i < P; ++i) {
            cin >> F[i];
            M[E[i]] += F[i];
        }

        vector<int> answer;
        int largest = M.rbegin()->first;

        auto remove = [&](int x, int y) {
            if ((M[x] -= y) == 0)
                M.erase(x);
        };
        remove(largest, 1);
        while (!M.empty()) {
            auto almost = M.rbegin()->first;
            int V = largest - almost;
            int K = answer.size();
            for (int i = 0; i < (1 << K); ++i) {
                int sum = 0;
                for (int j = 0; j < K; ++j)
                    if ((1 << j) & i)
                        sum += answer[j];
                remove(largest - sum - V, 1);
            }
            answer.insert(answer.begin(), V);
        }

        cout << "Case #" << test << ": ";
        reverse(answer.begin(), answer.end());
        for (auto &v : answer)
            cout << v << " ";
        cout << "\n";
    }
}


