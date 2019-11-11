#include <iostream>
#include <string>
#include <vector>
using namespace std;

void printAnswer(const vector<string>& vs) {
    for (size_t i = 0; i < vs.size(); i++) {
        cout << vs[i] << endl;
    }
}

int main() {

    int T;
    cin >> T;
    for (int t = 1; t <= T; t++) {
        int R, C, M;
        cin >> R >> C >> M;

        vector<string> ans(R, string(C, '.'));
        ans[0][0] = 'c';
        int row = R-1;
        int col = C-1;
        while (M > 0) {
            if (row > col) {
                for (int i = 0; i < C && ans[row][i] == '.' && M > 0; i++) {
                    ans[row][i] = '*';
                    M--;
                }
                row--;
            } else {
                for (int i = 0; i < R && ans[i][col] == '.' && M > 0; i++) {
                    ans[i][col] = '*';
                    M--;
                }
                col--;
            }
        }

        int dx[3] = {0, 1, 1};
        int dy[3] = {1, 1, 0};
        int total = 0;
        int mine = 0;
        for (int i = 0; i < 3; i++) {
            if (dx[i] < C && dy[i] < R) {
                total++;
                if (ans[dy[i]][dx[i]] == '*') {
                    mine++;
                }
            }
        }

        cout << "Case #" << t << ":" << endl;
        if (mine == 0 || mine == total) {
            printAnswer(ans);
        } else {
            cout << "Impossible" << endl;
        }
    }

    return 0;
}

