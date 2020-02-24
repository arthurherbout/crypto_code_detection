#include <iostream>
#include <string>
#include <vector>
using namespace std;

#define UP 0
#define DOWN 1
#define RIGHT 2
#define LEFT 3
int DX[4] = {0, 0, +1, -1};
int DY[4] = {-1, +1, 0, 0};

int toDirection(char c) {
    switch (c) {
        case '^':
            return UP;
        case 'v':
            return DOWN;
        case '<':
            return LEFT;
        case '>':
            return RIGHT;
    }
    return -1;
}

bool checkOK(const vector<string>& s, int row, int column, int dir) {
    int R = s.size();
    int C = s[0].length();
    int dx = DX[dir];
    int dy = DY[dir];
    int i = row+dy;
    int j = column+dx;
    while (i >= 0 && i < R && j >= 0 && j < C) {
        if (s[i][j] != '.') {
            return true;
        }
        i += dy;
        j += dx;
    }
    return false;
}

int solve(const vector<string>& s) {

    int r = s.size();
    int c = s[0].length();
    int ans = 0;
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            if (s[i][j] == '.') {
                continue;
            }
            int dir = toDirection(s[i][j]);
            if (checkOK(s, i, j, dir)) {
                continue;
            }
            bool canBeFixed = false;
            for (int tryDir = 0; tryDir < 4; tryDir++) {
                if (checkOK(s, i, j, tryDir)) {
                    canBeFixed = true;
                    break;
                }
            }
            if (!canBeFixed) {
                return -1;
            }
            ans++;
        }
    }
    return ans;
}

int main() {

    int T;
    cin >> T;

    for (int t = 1; t <= T; t++) {
        int R, C;
        cin >> R >> C;
        vector<string> a(R);
        for (int i = 0; i < R; i++) {
            cin >> a[i];
        }
        cout << "Case #" << t << ": ";
        int ans = solve(a);
        if (ans < 0) {
            cout << "IMPOSSIBLE" << endl;
        } else {
            cout << ans << endl;
        }
    }

    return 0;
}

