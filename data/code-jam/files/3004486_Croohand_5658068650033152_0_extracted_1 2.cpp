#include <iostream>
#include <iomanip>
#include <cstring>
#include <string>
#include <algorithm>
#include <queue>
#include <map>
#include <deque>
#include <set>
#include <vector>
#include <cstdlib>
#include <iomanip>
#include <ctime>
#include <fstream>

#define pb push_back
#define mp make_pair
#define F first
#define S second

#ifdef LOCAL
    #define debug(...) fprintf(stderr, __VA_ARGS__)
#else
    #define debug(...)
#endif // LOCAL

using namespace std;

typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;
typedef long double ld;

const int INF = int(1e9);
const ll INFll = 1ll * INF * INF;
const int MOD = 1000000007;

int t, n, m, k, a[21][21], d[21][21];

void dfs(int x, int y) {
    if (x == n || y == m || x < 0 || y < 0 || d[x][y] || a[x][y])
        return;
    d[x][y] = t;
    dfs(x + 1, y), dfs(x - 1, y), dfs(x, y + 1), dfs(x, y - 1);
}

bool solve(int mask) {
    int e = 0;
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j) {
            d[i][j] = 0;
            if (mask & (1 << (i * m + j)))
                a[i][j] = 1, e++;
            else
                a[i][j] = 0;
        }
    t = 0;
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            if (d[i][j] == 0) {
                t++;
                dfs(i, j);
            }
    vi u(t + 1, 1);
    for (int i = 0; i < n; ++i)
        u[d[i][0]] = 0, u[d[i][m - 1]] = 0;
    for (int i = 0; i < m; ++i)
        u[d[0][i]] = 0, u[d[n - 1][i]] = 0;
    u[0] = 0;
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            e += u[d[i][j]];
    return (e >= k);
}

int cnt(int x) {
    int k = 0;
    while (x) {
        x = x & (x - 1);
        k++;
    }
    return k;
}

int main() {
    cin.tie(0);
    ios_base::sync_with_stdio(0);
    #ifdef LOCAL
        freopen("C-small-attempt0.in", "r", stdin);
        //freopen("input.txt", "r", stdin);
        freopen("output.txt", "w", stdout);
    #endif
    int t;
    cin >> t;
    for (int o = 1; o <= t; ++o) {
        cout << "Case #" << o << ": ";
        cin >> n >> m >> k;
        int ans = INF;
        for (int i = 1; i < (1 << (n * m)); ++i)
            if (solve(i))
                ans = min(ans, cnt(i));
        cout << ans << endl;
    }
    return 0;
}

