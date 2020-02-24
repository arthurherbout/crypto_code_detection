#include <bits/stdc++.h>

#ifndef LOCAL
#define cerr dolor_sit_amet
#endif

#define mp make_pair
#define sz(x) ((int)((x).size()))
#define X first
#define Y second

using namespace std;
typedef long long ll;
typedef unsigned long long ull;
typedef long double ld;
typedef pair < int , int > ipair;
typedef pair < ll , ll > lpair;
const int IINF = 0x3f3f3f3f;
const ll LINF = 0x3f3f3f3f3f3f3f3fll;
const double DINF = numeric_limits<double>::infinity();
const int MOD = 1000000007;
const double EPS = 1e-9;
const int DX[] = { 1,  0, -1,  0,  1, -1,  1, -1};
const int DY[] = { 0,  1,  0, -1,  1, -1, -1,  1};
ll gcd(ll a, ll b) { return b ? gcd(b, a % b) : a; }
ll sqr(ll x) { return x*x; } ll sqr(int x) { return (ll)x*x; }
double sqr(double x) { return x*x; } ld sqr(ld x) { return x*x; }
mt19937 mmtw(960172);
ll rnd(ll x, ll y) { static uniform_int_distribution<ll> d; return d(mmtw) % (y - x + 1) + x; }

// ========================================================================= //

vector < vector < ipair > > pstacks;

int n, c;
vector < vector < ipair > > a;

bool go() {
    bool win = 1;
    for (auto const& x : a)
        if (sz(x) > 1)
            win = 0;
    if (win)
        return 1;

    for (int suit = 1; suit <= 4; ++suit) {
        int cnt = 0, mn = -1;
        for (int i = 0; i < n; ++i) {
            if (a[i].empty() || a[i].back().Y != suit)
                continue;
            ++cnt;
            if (mn == -1 || a[mn].back() > a[i].back())
                mn = i;
        }
        if (cnt > 1) {
            ipair q = a[mn].back();
            a[mn].pop_back();
            if (go()) return 1;
            a[mn].push_back(q);
        }
    }

    for (int i = 0; i < n; ++i)
        if (a[i].empty())
            for (int j = 0; j < n; ++j)
                if (sz(a[j]) > 1) {
                    a[i].push_back(a[j].back());
                    a[j].pop_back();
                    if (go()) return 1;
                    a[j].push_back(a[i].back());
                    a[i].clear();
                }
    return 0;
}

void solve() {
    cin >> n >> c;
    a.resize(n);
    for (int i = 0; i < n; ++i) {
        int xx;
        cin >> xx;
        a[i] = pstacks[xx];
    }

    if (go()) {
        cout << "POSSIBLE\n";
    } else {
        cout << "IMPOSSIBLE\n";
    }
}

int main() {
    ios::sync_with_stdio(false);
    cout.precision(20);

    int pp;
    cin >> pp;
    pstacks.resize(pp);
    for (int i = 0; i < pp; ++i) {
        int cc;
        cin >> cc;
        pstacks[i].resize(cc);
        for (int j = cc - 1; j >= 0; --j) {
            cin >> pstacks[i][j].X >> pstacks[i][j].Y;
        }
    }

    int t;
    cin >> t;
    for (int i = 1; i <= t; ++i) {
        cout << "Case #" << i << ": ";
        solve();
        cerr << "                                                                                                                                      Case " << i << "/" << t << " done\n";
    }

    return 0;
}
