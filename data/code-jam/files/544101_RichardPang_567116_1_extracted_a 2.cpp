#include <iostream>
#include <string>
#include <sstream>
#include <math.h>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <algorithm>
#include <numeric>
#include <string.h>
using namespace std;

int m, n, k;
int dr[] = {-1, -1, -1, 0 };
int dc[] = {-1, 0, 1, -1 };

vector<string> rot;

bool check2(int r, int c, int d) {
    int er = r + dr[d] * (k - 1);
    int ec = c + dc[d] * (k - 1);
    if (er < 0 || er >= m || ec < 0 || ec >= n) return 0;
    for (int i = 1; i < k; i++)
        if (rot[r + dr[d] * i][c + dc[d] * i] != rot[r][c]) return 0;
    return 1;
}

bool check(int r, int c) {
    for (int i = 0; i < 4; i++) if (check2(r, c, i)) return 1;
    return 0;
}

int main()
{
    int nc; cin >> nc; for (int cc = 0; cc < nc; cc++) {
        cin >> n >> k; vector<string> vs(n);
        for (int i = 0; i < n; i++) cin >> vs[i]; 
        m = vs[0].length(); rot = vector<string>(m, string(n, '.'));
        for (int i = 0; i < m; i++) for (int j = 0; j < n; j++) rot[i][j] = vs[n - 1 - j][i];

//        for (int i = 0; i < m; i++) cout << rot[i] << endl;

        for (int j = 0; j < n; j++) {
            for (int i = m - 1; i >= 0; i--) {
                bool found = 0;
                for (int ii = i - 1; ii >= 0; ii--) if (rot[ii][j] != '.') found = 1;
                if (found) while (rot[i][j] == '.') {
                    for (int kk = i; kk > 0; kk--) rot[kk][j] = rot[kk - 1][j];
                    rot[0][j] = '.';
                }
            }
        }

        bool okR = 0; bool okB = 0;
        for (int i = 0; i < m; i++) for (int j = 0; j < n; j++) {
            if (!okR && rot[i][j] == 'R') okR = check(i, j);
            if (!okB && rot[i][j] == 'B') okB = check(i, j);
        }
        cout << "Case #" << cc + 1 << ": ";
        if (!okR && !okB) cout << "Neither" << endl;
        else if (okR && okB) cout << "Both" << endl;
        else if (okR) cout << "Red" << endl;
        else cout << "Blue" << endl;
//        for (int i = 0; i < m; i++) cout << rot[i] << endl;
    }
    return 0;
}
