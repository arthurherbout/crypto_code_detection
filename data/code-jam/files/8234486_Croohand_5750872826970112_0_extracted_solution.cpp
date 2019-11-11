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
    cout << fixed << setprecision(7);
    for (int O = 1; O <= T; ++O) {
        cout << "Case #" << O << ": ";
        int n;
        cin >> n;
        ld v, c;
        cin >> v >> c;
        if (n == 1) {
            ld v1, c1;
            cin >> v1 >> c1;
            if (c1 != c)
                cout << "IMPOSSIBLE\n";
            else
                cout << v / v1 << endl;
            continue;
        }
        ld v1, c1, v2, c2;
        cin >> v1 >> c1 >> v2 >> c2;
        if (c1 == c2 && c1 == c) {
            cout << v / (v1 + v2) << endl;
            continue;
        }
        if (c1 > c && c2 > c || c1 < c && c2 < c)
            cout << "IMPOSSIBLE\n";
        else {
            ld t1 = v * (c - c2);
            t1 /= v1 * (c1 - c2);
            ld t2 = (v - v1 * t1) / v2;
            cout << max(t1, t2) << endl;
        }
    }
    debug("\nTIME = %ld\n", clock());
    return 0;
}

