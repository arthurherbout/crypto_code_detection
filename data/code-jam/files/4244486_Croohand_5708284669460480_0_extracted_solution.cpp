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
#include <ctime>
#include <fstream>

#define pb push_back
#define mp make_pair
#define F first
#define S second

#define foreach(i, c) for (typeof(c.begin()) i = c.begin(); i != c.end(); ++i)

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

ld f[103][103][103];

int main() {
    cin.tie(0);
    ios_base::sync_with_stdio(0);
#ifdef LOCAL
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif // LOCAL
    int T;
    cin >> T;
    cout << fixed << setprecision(7);
    for (int O = 1; O <= T; ++O) {
        cout << "Case #" << O << ": ";
        int c, n, m;
        cin >> c >> n >> m;
        string letters;
        cin >> letters;
        string s;
        cin >> s;
        s += '#';
        vi p(s.size());
        for (int i = 1; i < s.size(); ++i) {
            int x = p[i - 1];
            while (x && s[x] != s[i])
                x = p[x - 1];
            x += s[x] == s[i];
            p[i] = x;
        }
        vector<vi> aut(s.size(), vi(c));
        for (int i = 0; i <= n; ++i)
            for (int j = 0; j < c; ++j)
                if (s[i] == letters[j])
                    aut[i][j] = i + 1;
                else
                    aut[i][j] = i == 0 ? 0 : aut[p[i - 1]][j];
        for (int x = 0; x <= 101; ++x)
            for (int y = 0; y <= 101; ++y)
                for (int z = 0; z <= 101; ++z)
                    f[x][y][z] = 0;
        f[0][0][0] = 1;
        for (int i = 0; i < m; ++i)
            for (int j = 0; j <= n; ++j)
                for (int z = 0; z <= i; ++z) {
                    ld v = f[i][j][z];
                    if (v == 0)
                        continue;
                    for (int x = 0; x < c; ++x) {
                        int nj = aut[j][x];
                        int nz = z;
                        if (nj == n)
                            nz++;
                        f[i + 1][nj][nz] += v / ld(c);
                    }
                }
        ld res = 0;
        int cur = 0;
        for (int i = 0; i < m; ++i) {
            int mx = 0;
            for (int j = 0; j < c; ++j)
                if (aut[cur][j] > aut[cur][mx])
                    mx = j;
            cur = aut[cur][mx];
            if (cur == n)
                res += 1;
        }
        for (int i = 0; i <= n; ++i)
            for (int j = 0; j <= m; ++j)
                if (f[m][i][j] > 0)
                    res -= f[m][i][j] * j;
        cout << res << endl;
    }
    debug("\nTIME = %ld\n", clock());
    return 0;
}

