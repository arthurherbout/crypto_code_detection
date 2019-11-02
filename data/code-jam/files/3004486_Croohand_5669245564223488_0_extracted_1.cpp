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

int main() {
    cin.tie(0);
    //ios_base::sync_with_stdio(0);
    #ifdef LOCAL
        freopen("B-small-attempt0.in", "r", stdin);
        //freopen("input.txt", "r", stdin);
        freopen("output.txt", "w", stdout);
    #endif
    int t;
    cin >> t;
    for (int o = 1; o <= t; ++o) {
        cout << "Case #" << o << ": ";
        int ans = 0;
        int n;
        cin >> n;
        vector<string> a(n);
        for (int i = 0; i < n; ++i)
            cin >> a[i];
        vi p(n);
        for (int i = 0; i < n; ++i)
            p[i] = i;
        do {
            char c = 'A';
            vi q(26);
            bool f = 1;
            for (int i = 0; i < n; ++i) {
                if (!f)
                    break;
                for (int j = 0; j < a[p[i]].size(); ++j) {
                    if (a[p[i]][j] != c || c == 'A') {
                        c = a[p[i]][j];
                        if (q[c - 'a']) {
                            f = 0;
                            break;
                        }
                        q[c - 'a']++;
                    }
                }
            }
            ans += f;
        }
        while (next_permutation(p.begin(), p.end()));
        cout << ans << endl;
    }
    return 0;
}

