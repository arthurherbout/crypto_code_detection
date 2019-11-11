#include <bits/stdc++.h>

using namespace std;

pair <string, string> s[16];
int n;

bool getBit(int state, int pos) {
    return (state >> pos) & 1;
}

bool ok(int mask) {
    set <string> s1, s2;
    for (int i = 0; i < n; i++) {
        if (!getBit(mask, i)) {
            s1.insert(s[i].first);
            s2.insert(s[i].second);
        }
    }
    for (int i = 0; i < n; i++) {
        if (getBit(mask, i)) {
            if (s1.find(s[i].first) == s1.end() || s2.find(s[i].second) == s2.end()) return 0;
        }
    }
    return 1;
}

int solve() {
    int maxMask = (1 << n) - 1, res = 0;
    for (int mask = 0; mask <= maxMask; mask++) {
        int cnt = __builtin_popcount(mask);
        if (ok(mask)) res = max(res, cnt);
    }
    return res;
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    
    ios::sync_with_stdio(0);
    int T; cin >> T;
    for (int te = 1; te <= T; te++) {
        cin >> n;
        for (int i = 0; i < n; i++) cin >> s[i].first >> s[i].second;
        cout << "Case #" << te << ": " << solve() << endl;
    }
    
    return 0;
}
