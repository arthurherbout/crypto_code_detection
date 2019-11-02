#include <bits/stdc++.h>

using namespace std;

#define pb push_back
#define sz(a) int(a.size())

double p[20], y[20], no[20], res, cur;
int n, k;

bool getBit(int mask, int pos) {
    return (mask >> pos) & 1;
}

void dfs(int pos, double P, int Y, int N, vector <int> & idx) {
    if (pos == k) {
        if (Y == N) 
            cur += exp(P);
        return;
    }
    dfs(pos+1, P + y[idx[pos]], Y+1, N, idx);
    dfs(pos+1, P + no[idx[pos]], Y, N+1, idx);
}

void solve() {
    int maxMask = (1 << n) - 1;
    res = 0;
    for (int mask = 1; mask <= maxMask; mask++) {
        if (__builtin_popcount(mask) != k) continue;
        vector <int> idx;
        for (int i = 0; i < n; i++)
            if (getBit(mask, i)) idx.pb(i);
        cur = 0;
        dfs(0, 0, 0, 0, idx);
        res = max(res, cur);
    }
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    
    ios::sync_with_stdio(0);
    int T; cin >> T;
    for (int te = 1; te <= T; te++) {
        cin >> n >> k;
        res = 0;
        for (int i = 0; i < n; i++) cin >> p[i], y[i] = log(p[i]), no[i] = log(1 - p[i]);
        solve();
        cout << fixed << setprecision(9) << "Case #" << te << ": " << res << endl;
    }
    
    return 0;
}
