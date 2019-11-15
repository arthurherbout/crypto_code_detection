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

int main() {
    cin.tie(0);
    ios_base::sync_with_stdio(0);
#ifdef LOCAL
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif // LOCAL
    int T;
    cin >> T;
    for (int O = 1; O <= T; ++O) {
        cout << "Case #" << O << ": ";
        int c, n, v;
        cin >> c >> n >> v;
        vi a(n);
        for (int i = 0; i < n; ++i)
            cin >> a[i];
        if (c == 2) {
            cout << 1 << endl;
            continue;
        }
        vi f(v + 10000);
        f[0] = 1;
        for (int i = 0; i < n; ++i)
            for (int j = v; j >= 0; --j)
                if (f[j])
                    f[j + a[i]] = 1;
        int ans = 0;
        vi used(v + 1);
        for (int i = 0; i < n; ++i)
            used[a[i]] = 1;
        while (1) {
            int need = 0;
            for (int i = 0; i <= v; ++i)
                if (!f[i]) {
                    for (int j = 1; j <= v; ++j)
                        if (!used[j]) {
                            need = j;
                            break;
                        }
                    break;
                }
            if (!need)
                break;
            used[need] = 1;
            for (int i = v; i >= 0; --i)
                if (f[i])
                    f[i + need] = 1;
            ans++;
        }
        for (int x = 1; x <= 30; ++x) {
            f.assign(v + 10000, 0);
            f[0] = 1;
            for (int i = 0; i < n; ++i)
                for (int j = v; j >= 0; --j)
                    if (f[j])
                        f[j + a[i]] = 1;
            used.assign(v + 1, 0);
            for (int i = 0; i < n; ++i)
                used[a[i]] = 1;
            int cur = 0;
            bool ok = 1;
            while (1) {
                int need = 0;
                for (int i = 0; i <= v; ++i)
                    if (!f[i]) {
                        ok = 0;
                        for (int j = x; j >= 1; --j)
                            if (!used[j] && i - j >= 0 && f[i - j]) {
                                need = j;
                                ok = 1;
                                break;
                            }
                        break;
                    }
                if (!need)
                    break;
                used[need] = 1;
                for (int i = v; i >= 0; --i)
                    if (f[i])
                        f[i + need] = 1;
                cur++;
            }
            if (ok)
                ans = min(ans, cur);
        }
        cout << ans << endl;
    }
    return 0;
}

