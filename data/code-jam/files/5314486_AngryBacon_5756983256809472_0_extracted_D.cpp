#include <cmath>
#include <cstdio>
#include <cctype>
#include <cstdlib>
#include <climits>
#include <cstring>
#include <vector>
#include <string>
#include <iostream>
#include <cassert>
#include <algorithm>

using namespace std;

const int N = 100 + 5;
const int TT = N * N;
const int dx[] = {-1, 0, 1, 0};
const int dy[] = {0, 1, 0, -1};
namespace KM {
    // 最小匹配, 自带初始化 n <= m 方案存在 p[] 中
    const int N = 105;
    const int INF = 1000000000; // 严格大于最大边权
    long long n, m, a[N][N];
    long long u[N], v[N], p[N], fa[N], minv[N];
    bool used[N];
    int km() {
        memset(u, 0, sizeof(int) * n);
        for (int i = 0; i <= m; ++i) v[i] = 0, p[i] = n;
        for (int i = 0; i < n; ++i) {
            p[m] = i;
            int j0 = m;
            for (int j = 0; j <= m; ++j) minv[j] = INF, used[j] = false;
            do {
                used[j0] = true;
                int i0 = p[j0], delta = INF, j1;
                for (int j = 0; j < m; ++j) {
                    if (!used[j]) {
                        int cur = a[i0][j] - u[i0] - v[j];
                        if (cur < minv[j]) minv[j] = cur, fa[j] = j0;
                        if (minv[j] < delta) delta = minv[j], j1 = j;
                    }
                }
                for (int j = 0; j <= m; ++j) {
                    if (used[j]) {
                        u[p[j]] += delta, v[j] -= delta;
                    } else {
                        minv[j] -= delta;
                    }
                }
                j0 = j1;
            } while (p[j0] != n);
            do {
                int j1 = fa[j0];
                p[j0] = p[j1];
                j0 = j1;
            } while (j0 != m);
        }
        return -v[m];
    }
};


int n, m, k;
char a[N][N];
int id[N][N];
int dist[N][N];

void solve()
{
    cin >> m >> n >> k;
    int tots = 0;
    int tott = 0;

    for(int i = 0; i < n; ++ i) {
        scanf("%s", a[i]);
        for(int j = 0; j < m; ++ j) {
            if (a[i][j] == 'S') {
                id[i][j] = tots ++;
            }
            if (a[i][j] == 'T') {
                id[i][j] = tott ++;
            }
        }
    }
    KM::n = tots;
    KM::m = max(tott, tots);
    for(int i = 0; i < KM::n; ++ i) {
        for(int j = 0; j < KM::m; ++ j) {
            KM::a[i][j] = KM::INF - 10;
        }
    }

    for(int i = 0; i < n; ++ i) {
        for(int j = 0; j < m; ++ j) {
            if (a[i][j] == 'S') {
                vector<pair<int, int>> que;
                que.push_back(make_pair(i, j));
                memset(dist, -1, sizeof dist);
                dist[i][j] = 0;
                for(int w = 0; w < que.size(); ++ w) {
                    int x = que[w].first;
                    int y = que[w].second;
                    for(int d = 0; d < 4; ++ d) {
                        int tx = x, ty = y;
                        while(! (tx < 0 || tx >= n || ty < 0 || ty >= m)) {
                            if (a[tx][ty] == '#') break;
                            if (a[tx][ty] == 'T') {
                                int u = id[i][j];
                                int v = id[tx][ty];
                                KM::a[u][v] = min(KM::a[u][v], 0ll + dist[x][y] - TT);
                            }
                            tx += dx[d];
                            ty += dy[d];
                        }
                    }
                    if (dist[x][y] >= k) break;
                    for(int d = 0; d < 4; ++ d) {
                        int nx = x + dx[d];
                        int ny = y + dy[d];
                        if (nx < 0 || nx >= n || ny < 0 || ny >= m) continue;
                        if (a[nx][ny] == '#') continue;
                        if (dist[nx][ny] != -1) continue;
                        dist[nx][ny] = dist[x][y] + 1;
                        que.push_back(make_pair(nx, ny));
                    }
                }
            }
        }
    }

    KM::km();
    vector<pair<int, int>> ans;
    for(int j = 0; j < tott; ++ j) {
        int i = KM::p[j];
        if (KM::a[i][j] < KM::INF - 10) {
            ans.push_back(make_pair(i, j));
        }
    }
    cout << ans.size() << endl;
    for(auto e: ans) {
        cout << e.first + 1 << ' ' << e.second + 1 << endl;
    }
}

int main()
{
    //freopen("D-small-attempt0.in", "r", stdin); freopen("D-small-attempt0.out", "w", stdout);
    freopen("D-small-attempt1.in", "r", stdin); freopen("D-small-attempt1.out", "w", stdout);
    //freopen("D-small-attempt2.in", "r", stdin); freopen("D-small-attempt2.out", "w", stdout);
    //freopen("D-large.in", "r", stdin); freopen("D-large.out", "w", stdout);
    int test_case;
    cin >> test_case;
    for(int i = 0; i < test_case; ++ i) {
        printf("Case #%d: ", i + 1);
        cerr << "Start: " << i << endl;
        solve();
    }
    return 0;
}
