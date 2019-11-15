#include <bits/stdc++.h>
using namespace std;
#define forn(i, n) for (int i = 0; i < (int)(n); ++i)
#define fore(i, b, e) for (int i = (int)(b); i <= (int)(e); ++i)
#define ford(i, n) for (int i = (int)(n) - 1; i >= 0; --i)
#define pb push_back
#define fi first
#define se second
#define all(x) (x).begin(), (x).end()
typedef vector<int> vi;
typedef pair<int, int> pii;
typedef long long i64;
typedef unsigned long long u64;
typedef long double ld;
typedef long long ll;

const int maxn = 100500;

#define prev toarktoartarstarsotiar

set<pii> cur, nxt;
vector<pii> trans(pii p) {
    int f0, f1;
    tie(f0, f1) = p;
    return {
        {f0+2*f1,f0+3*f1},
        {f0+2*f1, f1},
        {2*f0+3*f1,f0+2*f1},
        {2*f1+f0,2*f1+f0},
        {f1, f1},
        {2*f1, f1},
        {2*f1, 3*f1},
        {4*f0+4*f1, 4*f0+4*f1},
        {2*f0+3*f1, 3*f0+5*f1}
        // {4*f0+4*f1, 8*f0+8*f1}
    };
}

map<pii, pair<pii, int>> prev;
map<int, pii> res;

int gaub(vector<vector<ld>> a) {
    int n = a.size();
    ld det = 1;
    forn(i, n) {
        int v = i;
        for (int j = i; j < n; ++j) if (abs(a[j][i]) > abs(a[v][i])) v = j;
        swap(a[v], a[i]);
        if (!(abs(a[i][i]) > 1e-9)) return -1;
        det *= a[i][i];
        for (int j = i+1; j < n; ++j) {
            ld cf = a[j][i] / a[i][i];
            forn(k, n) a[j][k] -= a[i][k] * cf;
        }
    }
    assert(fabsl(round(det) - det) < 1e-5);
    return abs(round(det));
}

void go() {
    cur.emplace(1, 1);
    cur.emplace(1, 0);
    forn(i, 10) {
        for (auto t: cur) {
            auto to = trans(t);
            for (auto val: to) {
                if (max(val.fi, val.se) < 100000) {
                    nxt.insert(val);
                }
            }
        }
        for (auto t: nxt) if (t.se <= 10000) res[t.se] = {1,1};
        cur = nxt;
        nxt.clear();
    }
    cerr << res.size() << endl;
    fore(i, 2, 10000) if (!res.count(i)) {
        cerr << i << endl;
    }
    cur.clear();
    res.clear();
}

void run() {
    queue<pii> q;
    q.emplace(1, 1);
    res[1] = {1, 1};
    cur.insert({1, 1});
    q.emplace(1, 0);
    cur.insert({1, 0});
    res[2] = {1, 1};
    while (!q.empty()) {
        auto p = q.front();
        q.pop();
        auto tr = trans(p);
        forn(i, tr.size()) {
            auto to = tr[i];
            if (max(to.fi, to.se) < 100000 && !cur.count(to)) {
                cur.insert(to);
                q.push(to);
                if (to.se <= 10000 && to.se > 0 && !res.count(to.se)) res[to.se] = to;
                prev[to] = { p, i };
            }
        }

        if (res.size() >= 9906) break; // 10000?
    }
}

int a[100][100];
int n;

void clear() {
    memset(a, 0, sizeof a);
    n = 0;
}

int check() {
    vector<vector<ld>> b(n, vector<ld>(n, 0));
    forn(i, n) forn(j, i) if (a[i][j]) {
        b[i][j] = b[j][i] = 1;
        b[i][i] -= 1;
        b[j][j] -= 1;
    }
    forn(i, n) b[i].pop_back();
    b.pop_back();
    return gaub(b);
}

void finalize() {
    forn(i, n) {
        int cnt = 0;
        forn(j, n) cnt += a[i][j];
        if (cnt == 0) a[i][0] = a[0][i] = 1;
    }
}

void ae(int i, int j) {
    a[i+n][j+n] = a[j+n][i+n] = 1;
}

void restore(pii p) {
    if (!prev.count(p)) {
        clear();
        if (p == pii{1, 1}) {
            // cerr << "yep\n";
            ae(0, 1);
        } else {
            // cerr << "nope\n";
        }
        return;
    }
    restore(prev[p].first);
    int id = prev[p].second;
    // cerr << "id = " << id << endl;
    if (id == 0) ae(0, 2), ae(1, 3), ae(2, 3);
    if (id == 1) ae(0, 2), ae(1, 3);
    if (id == 2) ae(0, 2), ae(1, 3), ae(1, 2);
    if (id == 3) ae(0, 2), ae(1, 2), ae(2, 3);
    if (id == 4) ae(0, 2), ae(2, 3);
    if (id == 5) ae(0, 2), ae(0, 3);
    if (id == 6) ae(0, 2), ae(0, 3), ae(2, 3);
    if (id == 7) ae(0, 2), ae(1, 2), ae(0, 3), ae(1, 3);
    if (id == 8) ae(0, 2), ae(0, 3), ae(1, 3), ae(2, 3);
    n += 2;
}

map<int, vector<vi>> answers;

void setAns(int x) {
    vector<vi> a(n, vi(n));
    forn(i, n) forn(j, n) a[i][j] = ::a[i][j];
    answers[x] = a;
}

void solve_all() {
    run();
    cerr << "done\n";
    set<int> results;
    fore(i, 3, 10000) if (res.count(i)) {
        if (i%100 == 0) cerr << i << endl;
        restore(res[i]);
        n += 2;
        assert(check() == i);
        assert(n <= 22);

        results.insert(i);
        setAns(i);

        forn(i, n) forn(j, i) {
            a[i][j] ^= 1;
            a[j][i] ^= 1;
            int t = check();
            if (t == 122) {
                cerr << "YES!!!!!\n";
            }
            if (t <= 10000 && !results.count(t)) results.insert(t), setAns(t);

            // forn(k, j) {
            //     a[i][k] ^= 1;
            //     a[k][i] ^= 1;
            //     int t = check();
            //     if (t == 122) {
            //         cerr << "YES!!!!!\n";
            //     }
            //     if (t <= 10000 && !results.count(t)) results.insert(t), setAns(t);
            //     a[i][k] ^= 1;
            //     a[k][i] ^= 1;
            // }
            a[i][j] ^= 1;
            a[j][i] ^= 1;
        }
    }
    results.erase(-1);
    // cout << results.size() << endl;
}

void solve() {
    run();
    cerr << "done" << endl;
    int nn;
    cin >> nn;
    forn(i, nn) {
        int k;
        cin >> k;
        assert(res.count(k));
        restore(res[k]);
        n += 2;
        cout << n << endl;
        forn(i, n) {
            forn(j, n) cout << a[i][j];
            cout << "\n";
        }
        cout << check() << endl;
        cout << "\n";
    }
}

void solve122() {
    cout << "10\n\
0110000010\n\
1001000000\n\
1000100000\n\
0100010000\n\
0010011000\n\
0001100100\n\
0000100001\n\
0000010001\n\
1000000001\n\
0000001110" << endl;
}

void solvegcj() {
    solve_all();
    int t;
    cin >> t;
    forn(i, t) {
        int n;
        cin >> n;
        if (n == 122) {
            cout << "Case #" << i+1 << ": ";
            solve122();
            continue;
        }
        auto a = answers[n];
        assert(!a.empty());
        cout << "Case #" << i+1 << ": " << a.size() << endl;
        forn(i, a.size()) {
            forn(j, a.size()) cout << a[i][j];
            cout << endl;
        }
    }
}

int main() {
#ifdef LOCAL
    // freopen("input.txt", "r", stdin);
#endif

    solvegcj();
    // solve_all();

    // go();

    // solve();

    // go();
    // run();


    // forn(i, 2) if (i) {
    //     clear();
    //     if (i) ae(0, 1);
    //
    //     ae(0, 2);
    //     ae(2, 3);
    //
    //     n = 4;
    //     cout << check() << endl;
    // }
    //


#ifdef LOCAL
    cerr << "Time elapsed: " << clock() / 1000 << " ms" << endl;
#endif
    return 0;
}
