#include <bits/stdc++.h>

using namespace std;

#define pb push_back
#define sz(a) (int)a.size()

const int mv[2][2] = { {0, 1}, {1, 0} };
int r, c, n;

int get_bit(int mask, int pos) {
    return (mask >> pos) & 1;
}

vector <int> find_bit1(int mask) {
    vector <int> res;
    for (int i = 0; i < r * c; i++)
        if (get_bit(mask, i) == 1) res.pb(i);
    return res;
}

bool inside(int x, int y) {
    return x >= 0 && x < r && y >= 0 && y < c;
}

int get_unhappiness(vector <vector<int> > & board) {
    int res = 0;
    for (int i = 0; i < r; i++)
    for (int j = 0; j < c; j++) {
        if (board[i][j] == 0) continue;
        for (int k = 0; k < 2; k++) {
            int ii = i + mv[k][0], jj = j + mv[k][1];
            if (!inside(ii, jj) || board[ii][jj] == 0) continue;
            res++;
        }
    }
    return res;
}

int solve() {
    int num_cells = r * c, res = 1e9;
    for (int mask = 0; mask < (1 << num_cells); mask++) {
        vector <int> pos = find_bit1(mask);
        if (sz(pos) != n) continue;
        vector <vector<int> > board(r, vector<int>(c, 0));
        for (int i = 0; i < sz(pos); i++) {
            board[pos[i] / c][pos[i] % c] = 1;
        }
        int unhappiness = get_unhappiness(board);
        res = min(res, unhappiness);
    }
    return res;
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    int nTest; scanf("%d", &nTest);
    for (int t = 1; t <= nTest; t++) {
        scanf("%d%d%d", &r, &c, &n);
        printf("Case #%d: %d\n", t, solve());
    }

    return 0;
}
