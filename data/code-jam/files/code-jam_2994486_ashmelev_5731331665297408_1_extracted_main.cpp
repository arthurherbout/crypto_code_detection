#include <iostream>
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include <bitset>
#include <queue>
#include <stack>
#include <sstream>
#include <cstring>
#include <numeric>
#include <ctime>

#define re return
#define fi first
#define se second
#define mp make_pair
#define pb push_back
#define all(x) (x).begin(), (x).end()
#define sz(x) ((int) (x).size())
#define rep(i, n) for (int i = 0; i < (n); i++)
#define rrep(i, n) for (int i = (n) - 1; i >= 0; i--)
#define y0 y32479
#define y1 y95874
#define fill(x, y) memset(x, y, sizeof(x))
#define sqr(x) ((x) * (x))
#define sqrt(x) sqrt(abs(x))
#define unq(x) (x.resize(unique(all(x)) - x.begin()))
#define spc(i,n) " \n"[i == n - 1]

using namespace std;

typedef vector<int> vi;
typedef vector<vi> vvi;
typedef pair<int, int> ii;
typedef vector<ii> vii;
typedef vector<string> vs;
typedef double D;
typedef long double LD;
typedef long long ll;
typedef pair<ll, ll> pll;
typedef vector<ll> vll;

template<class T> T abs(T x) { return x > 0 ? x : -x;}

#define FILENAME ""

int m;
int n;

string s[100];
vi v[100];

int tw[100];
int w[100];
int e[100];

int st[100], sp;
int pp[100];

void dfs(int x) {
    tw[x] = 1;
    rep(i, sz(v[x])) {
        int y = v[x][i];
        if (tw[y] || e[y])
            continue;
        dfs(y);
    }
}

int checkEnd(int x) {
    int p = pp[x];
    if (p == -1)
        re 0;
    fill(tw, 0);
    dfs(p);
    rep(i, n)
    if (!w[i] && !tw[i])
        re 0;
    re 1;
}

int getmin(int x) {
    int bid = -1;
    rep(i, sz(v[x])) {
        int y = v[x][i];
        if (w[y])
            continue;
        if (bid == -1 || s[y] < s[bid])
            bid = y;
    }
    re bid;
}

int getmin2(int x, bool &ppp) {
    int bid = getmin(x);

    e[x] = 1;
    ppp = false;
    if (checkEnd(x)) {
        int prev = pp[x];
        bool tmp;
        int y = getmin2(prev, tmp);
        if (bid == -1 || s[y] < s[bid]) {
            ppp = true;
            bid = y;
        }
    }
    e[x] = 0;

    re bid;
}

string go(int x, bool add) {
    //cout << x << ' ' << add << endl;

    if (add) {
        w[x] = 1;
        st[sp++] = x;
    }

    int end = 1;
    rep(i, n)
        if (!w[i])
            end = 0;

    if (end)
        re "";

    bool ppp;
    int bid = getmin2(x, ppp);

    if (ppp) {
        sp--;
        e[x] = 1;
        re go(pp[x], 0);
    }
    else {
        pp[bid] = x;
        re s[bid] + go(bid, 1);
    }
}

string getans(int p) {
    //cout << p << endl;
    sp = 0;
    fill(pp, -1);
    fill(w, 0);
    fill(e, 0);
    pp[p] = -1;
    re s[p] + go(p, 1);
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    int tc;
    cin >> tc;
    rep(tt, tc) {
        cout << "Case #" << tt + 1 << ": ";

        cin >> n >> m;
        rep(i, n)
            cin >> s[i];
        rep(i, n)
            v[i].clear();
        rep(i, m) {
            int a, b;
            cin >> a >> b;
            a--;
            b--;
            v[a].pb(b);
            v[b].pb(a);
        }
        /*rep(i, n)
            s[i] = "11111";
        n = 50;
        rep(i, n)
            v[i].clear();
        rep(i, n) rep(j, n)
            if (i != j)
                v[i].pb(j);*/

        int bb = 0;
        rep(i, n)
            if (s[i] < s[bb])
                bb = i;
        string best = getans(bb);
        cout << best << endl;
    }

    re 0;
}
