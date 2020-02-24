#include <iostream>
#include <vector>
using namespace std;

int countNoise(const vector<vector<bool>>& a) {
    int r = a.size();
    int c = a[0].size();
    int ans = 0;
    for (int i = 0; i < r; i++) {
        for (int j = 1; j < c; j++) {
            if (a[i][j-1] && a[i][j]) {
                ans++;
            }
        }
    }
    for (int i = 1; i < r; i++) {
        for (int j = 0; j < c; j++) {
            if (a[i-1][j] && a[i][j]) {
                ans++;
            }
        }
    }
    return ans;
}

int solve(int R, int C, int N) {
    int totalSquares = R*C;
    int ans = 2000000000;
    for (int bit = 0; bit < (1<<totalSquares); bit++) {
        int count = 0;
        for (int i = 0; i < totalSquares; i++) {
            if (bit & (1<<i)) {
                count++;
            }
        }
        if (count != N) {
            continue;
        }
        vector<vector<bool>> a(R, vector<bool>(C, false));
        for (int i = 0; i < totalSquares; i++) {
            if (bit & (1<<i)) {
                a[i/C][i%C] = true;
            }
        }
        ans = min(ans, countNoise(a));
    }
    return ans;
}

int main() {

    int T;
    cin >> T;

    for (int t = 1; t <= T; t++) {
        int R, C, N;
        cin >> R >> C >> N;
        cout << "Case #" << t << ": " << solve(R, C, N) << endl;
    }

    return 0;
}

