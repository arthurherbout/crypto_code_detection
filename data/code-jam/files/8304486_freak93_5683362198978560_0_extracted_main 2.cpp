#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

int manhattan(int x1, int y1, int x2, int y2) {
    if (x1 > x2)
        swap(x1, x2);
    if (y1 > y2)
        swap(y1, y2);
    return x2 - x1 + y2 - y1;
}

static const int kModulo = 1000 * 1000 * 1000 + 7;

int main() {
    int T; cin >> T;

    for (int test = 1; test <= T; ++test) {
        int R, C, N, D; cin >> R >> C >> N >> D;

        vector< vector<int64_t> > min_value(R, vector<int64_t>(C, numeric_limits<int64_t>::min())),
                                  max_value(R, vector<int64_t>(C, numeric_limits<int64_t>::max()));

        for (int i = 0; i < N; ++i) {
            int Ri, Ci, Bi; cin >> Ri >> Ci >> Bi;
            --Ri; --Ci;
            for (int j = 0; j < R; ++j)
                for (int k = 0; k < C; ++k) {
                    min_value[j][k] = max(min_value[j][k], Bi - 1LL * D * manhattan(j, k, Ri, Ci));
                    max_value[j][k] = min(max_value[j][k], Bi + 1LL * D * manhattan(j, k, Ri, Ci));
                }
        }

        int answer = 0;
        bool good = true;
        for (int j = 0; j < R; ++j)
            for (int k = 0; k < C; ++k) {
                if (min_value[j][k] > max_value[j][k])
                    good = false;
                answer = (answer + max_value[j][k]) % kModulo;
            }
        cout << "Case #" << test << ": ";
        if (good)
            cout << answer << "\n";
        else
            cout << "IMPOSSIBLE\n";
    }
}
