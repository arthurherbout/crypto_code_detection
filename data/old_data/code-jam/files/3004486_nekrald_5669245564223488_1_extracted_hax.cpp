#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <memory.h>
#include <cassert>

using namespace std;

typedef long long ll;

const int SZ = 40, mSZ = 4, tail = 0, mid = 1, beg = 2, whl = 3;
int ltr[40][4];
int n, grct;

ll fact[200], MOD = 1000000007;

void init() {
    fact[0] = 1ll;
    for (ll i = 1; i < 200; ++i) {
        fact[i] = (fact[i - 1] * i) % MOD;
    }
}


bool chwrd(string t) {
    vector<pair<char, int> > vp;
    for (int i = 0; i < t.length(); ++i) vp.push_back(make_pair(t[i], i));
    sort(vp.begin(), vp.end());
    for (int i = 1; i < vp.size(); ++i) {
        if (vp[i].first == vp[i - 1].first && vp[i-1].second + 1 != vp[i].second) {
            return false;
        }
    }
    return true;
}

void build(string t, int ind) {
    char lt = t[0];
    bool same = true;
    for (int i = 0; i < t.length(); ++i) {
        if (t[i] != lt) same = false;
    }
    if (same) {
        ltr[lt - 'a'][whl]++;
        return;
    }
    char el = t[(int)t.length() - 1];
    ltr[lt - 'a'][beg]++;
    ltr[el - 'a'][tail]++;
    for (int pos = 1; pos < t.length(); ++pos) {
        if (t[pos] != lt && t[pos] != el && t[pos] != t[pos - 1]) {
            ltr[t[pos] - 'a'][mid]++;
        }
    }
}

ll answay(int letid) {
    ll ans = 1ll;
    if (ltr[letid][beg] >= 2 || ltr[letid][tail] >= 2 || ltr[letid][mid] >= 2) {
        ans *= 0ll;
    }
    int nmid = ltr[letid][beg] + ltr[letid][tail] + ltr[letid][whl];
    if (ltr[letid][mid] && nmid) ans *= 0ll;

    ans *= fact[ltr[letid][whl]];
    bool cond = (ltr[letid][beg] + ltr[letid][tail] + ltr[letid][mid] + ltr[letid][whl] > 0);

    if (!ltr[letid][tail] && !ltr[letid][mid] && cond) {
        ++grct;
    }
    if (!cond && ans != 1ll) assert(false);
    return ans;

}

int main() {
    init();
    std::ios_base::sync_with_stdio(false);
    int tcn; cin >> tcn;
    for (int _ = 1; _ <= tcn; ++_) {
        grct = 0; memset(ltr, 0, sizeof(ltr));
        cin >> n; bool inside = true;
        for (int i = 0; i < n; ++i) {
            string t; cin >> t;
            inside = inside && chwrd(t);
            build(t, i);
        }
        ll ans = inside ? 1ll : 0ll;
        for (int let = 'a' - 'a'; let <= 'z' - 'a'; ++let) {
            ans = (ans *  answay(let)) % MOD;
        }
        ans =  (ans * fact[grct]) % MOD;
        if (grct == 0) ans *= 0ll;
        cout << "Case #" << _ << ": " << ans << endl;
    }
    return 0;
}
