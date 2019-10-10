#include <cstdio>
#include <cassert>
#include <algorithm>
#include <set>
#include <vector>
using namespace std;

const int N = 105;

const int MX = 10000;

int curver = 0;
int was[N][MX];
set<int> unused[N];
int D[N][MX];

int X[N], Y[N], Z[N];

int dist(int i, int j) {
    return abs(X[i] - X[j]) + abs(Y[i] - Y[j]) + abs(Z[i] - Z[j]);
}

void solve(int cs) {
    ++curver;
    int n;
    scanf("%d", &n);
    n += 2;
    for (int i = 0; i < n; i++) {
        scanf("%d %d %d", &X[i], &Y[i], &Z[i]);
    }
    vector<pair<int, int>> Q;
    for (int i = 2; i < n; i++) {
        int d = dist(0, i);
        Q.emplace_back(i, d);
        was[i][d] = curver;
        D[i][d] = 0;
        unused[i].clear();
        for (int j = 0; j < MX; j++) {
            unused[i].insert(j);
        }
        unused[i].erase(d);
    }
    int lpt = 0;
    int ans = -1;
    while (lpt < Q.size()) {
        auto st = Q[lpt++];
        int i = st.first;
        int d = st.second;
        assert(unused[i].count(d) == 0);
        int dt = dist(1, i);
        if (dt == d) {
            ans = D[i][d] + 1;
            break;
        }
        for (int j = 2; j < n; j++) {
            if (j == i) {
                continue;
            }
            int p = dist(i, j);
            int lb = -1, rb = -1;
            if (p > d) {
                lb = p - d;
                rb = p + d;
            } else {
                lb = d - p;
                rb = d + p;
            }

            auto it = unused[j].lower_bound(lb);
            while (true) {
                if (it == unused[j].end())
                    break;
                int r = *it;
                if (r > rb) {
                    break;
                }
                assert(was[j][r] != curver);
                Q.emplace_back(j, r);
                was[j][r] = curver;
                D[j][r] = D[i][d] + 1;
                unused[j].erase(r);
                ++it;
            }
        }
    }
    if (ans != -1) {
        printf("Case #%d: %d\n", cs, ans);
    } else {
        printf("Case #%d: IMPOSSIBLE\n", cs);
    }
    fflush(stdout);
}

int main() {
    int T;
    scanf("%d", &T);
    for (int i = 0; i < T; i++) {
        solve(i + 1);
        fprintf(stderr, "%d\n", i);
    }
}
