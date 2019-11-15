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
        int n;
        cin >> n;
        string s;
        getline(cin, s);
        vector<vector<string> > b(n);
        for (int i = 0; i < n; ++i) {
            getline(cin, s);
            s += " ";
            string cur = "";
            for (int j = 0; j < s.size(); ++j)
                if (s[j] == ' ') {
                    if (!cur.empty())
                        b[i].pb(cur);
                    cur = "";
                } else
                    cur += s[j];
        }
        map<string, int> dict;
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < b[i].size(); ++j)
                dict[b[i][j]] = 1;
        int k = 0;
        foreach (i, dict)
            i->S = k++;
        vector<vi> a(n);
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < b[i].size(); ++j)
                a[i].pb(dict[b[i][j]]);
        int ans = INF;
        for (int mask = 0; (mask < (1 << (n - 2))); ++mask) {
            int cur = 0;
            vi eng(k);
            for (int i = 2; i < n; ++i)
                if (mask & (1 << (i - 2)))
                    for (int j = 0; j < a[i].size(); ++j)
                        eng[a[i][j]] = 1;
            for (int j = 0; j < a[0].size(); ++j)
                eng[a[0][j]] = 1;
            for (int i = 2; i < n; ++i)
                if ((mask & (1 << (i - 2))) == 0)
                    for (int j = 0; j < a[i].size(); ++j) {
                        if (cur > ans)
                            break;
                        if (eng[a[i][j]])
                            eng[a[i][j]] = 0, cur++;
                    }
            for (int j = 0; j < a[1].size(); ++j)
                if (eng[a[1][j]])
                    eng[a[1][j]] = 0, cur++;
            ans = min(ans, cur);
        }
        debug("%d\n", O);
        cout << ans << endl;
    }
    debug("\nTIME = %ld\n", clock());
    return 0;
}

