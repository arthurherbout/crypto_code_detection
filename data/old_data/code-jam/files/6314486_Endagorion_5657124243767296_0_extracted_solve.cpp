#include <iostream>
#include <tuple>
#include <sstream>
#include <vector>
#include <cmath>
#include <ctime>
#include <bitset>
#include <cassert>
#include <cstdio>
#include <queue>
#include <set>
#include <map>
#include <fstream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <algorithm>
#include <numeric>

#define mp make_pair
#define mt make_tuple
#define fi first
#define se second
#define pb push_back
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define forn(i, n) for (int i = 0; i < (int)(n); ++i)
#define for1(i, n) for (int i = 1; i <= (int)(n); ++i)
#define ford(i, n) for (int i = (int)(n) - 1; i >= 0; --i)
#define fore(i, a, b) for (int i = (int)(a); i <= (int)(b); ++i)

using namespace std;

typedef pair<int, int> pii;
typedef vector<int> vi;
typedef vector<pii> vpi;
typedef vector<vi> vvi;
typedef long long i64;
typedef vector<i64> vi64;
typedef vector<vi64> vvi64;
typedef pair<i64, i64> pi64;
typedef double ld;

template<class T> bool uin(T &a, T b) { return a > b ? (a = b, true) : false; }
template<class T> bool uax(T &a, T b) { return a < b ? (a = b, true) : false; }

const int maxa = 1000000, maxn = 51000;
vi e[maxa];
int match[maxn], m1[maxa];
i64 vis[maxa];
i64 GC = 0;

bool dfs(int v) {
    if (vis[v] == GC) return false;
    vis[v] = GC;
    for (int u: e[v]) {
        if (match[u] == -1 || dfs(match[u])) {
            m1[v] = u;
            match[u] = v;
            return true;
        }
    }
    return false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.precision(10);
    cout << fixed;

    int T;
    cin >> T;
    for1(tc, T) {
        forn(i, maxa) e[i].clear(), m1[i] = -1;
        int n;
        cin >> n;
        forn(i, n) match[i] = -1;
        forn(i, n) {
            forn(j, 6) {
                int x;
                cin >> x;
                --x;
                e[x].pb(i);
            }
        }

        forn(i, n) match[i] = -1;
        int r = 0;
        int ans = 0;
        forn(l, maxa) {
            uax(r, l);
            uax(ans, r - l + 1);
            while (r < maxa) {
                ++GC;
                if (dfs(r)) ++r;
                else break;
            }
            if (r > l) {
                int u = m1[l];
                match[u] = -1;
                m1[l] = -1;
            }
        }
        cout << "Case #" << tc << ": " << ans << '\n';
    }

#ifdef LOCAL_DEFINE
    cerr << "Time elapsed: " << 1.0 * clock() / CLOCKS_PER_SEC << " s.\n";
#endif
    return 0;
}
