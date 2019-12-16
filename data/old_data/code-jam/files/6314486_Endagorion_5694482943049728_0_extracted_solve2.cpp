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
#define forr(i, n, m) for (int i = (int)(n); i < (int)(m); ++i)
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

const int maxn = 23, maxa = 10001;

vector<double> f[maxn];

i64 trees(vvi a) {
    int n = a.size();
    forn(i, n) f[i].assign(n, 0.0);
    forn(i, n) forn(j, i) {
        if (a[i][j]) {
            ++f[i][i];
            ++f[j][j];
            --f[i][j];
            --f[j][i];
        }
    }

    double P = 1.0;
    --n;
    forn(i, n) {
        int j = i;
        forr(k, i, n) {
            if (fabs(f[k][i]) > fabs(f[j][i])) j = k;
        }
        swap(f[j], f[i]);
        if (fabs(f[i][i]) < 1e-9) return 0.0;
        double q = f[i][i];
        P *= q;
        forn(k, n) f[i][k] /= q;
        forr(j, i + 1, n) {
            double t = f[j][i];
            forn(k, n) f[j][k] -= t * f[i][k];
        }
    }
    uin(P, 1e9);
    return fabs(P) + 0.5;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.precision(10);
    cout << fixed;

    int T;
    cin >> T;
    for1(tc, T) {
        int k;
        cin >> k;
        cerr << tc << ' ' << k << '\n';
        int n = 22;
        vvi a(n, vi(n));
//        forn(i, n) forn(j, i) if (rand() % 2) a[i][j] = a[j][i] = 1;
        forn(i, n - 1) a[i][i + 1] = a[i + 1][i] = 1;
        a[0][n - 1] = a[n - 1][0] = 1;
        i64 T = trees(a);
        while (T != k) {
            if (T < k) {
                int i, j;
                while (1) {
                    i = rand() % n;
                    j = rand() % n;
                    if (i == j || a[i][j]) continue;
                    break;
                }
                a[i][j] = a[j][i] = 1;
                T = trees(a);
            } else {
                int i, j;
                while (1) {
                    i = rand() % n;
                    j = rand() % n;
                    if (i == j || !a[i][j]) continue;
                    a[i][j] = a[j][i] = 0;
                    T = trees(a);
                    if (!T) {
                        a[i][j] = a[j][i] = 1;
                        continue;
                    }
                    break;
                }
            }
//            if (k == 22) cerr << k << ' ' << T << '\n';
        }
        cout << "Case #" << tc << ": " << n << '\n';
        forn(i, n) {
            forn(j, n) cout << a[i][j];
            cout << endl;
        }
    }

#ifdef LOCAL_DEFINE
    cerr << "Time elapsed: " << 1.0 * clock() / CLOCKS_PER_SEC << " s.\n";
#endif
    return 0;
}

