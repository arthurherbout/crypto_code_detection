#include <bits/stdc++.h>

using namespace std;

#define pb push_back
#define sz(a) int(a.size())

string s[30];
int n;

bool getBit(int mask, int pos) {
    return (mask >> pos) & 1;
}

bool dfs(int pos, vector <int> & A, int mask, int used) {
    if (pos == sz(A)) {
        return 1;
    }
    int who = A[pos];
    bool gotTask = 0;
    for (int task = 0; task < n; task++) {
        if (getBit(used, task)) continue;
        if (s[who][task] == '1' || getBit(mask, who*n + task)) {
            gotTask = 1;
            if (!dfs(pos+1, A, mask, used | (1 << task)))
                return 0;
        }
    }
    if (!gotTask) return 0;
    return 1;
}

bool ok(int mask) {
    vector <int> A;
    for (int i = 0; i < n; i++) A.pb(i);
    do {
        if (!dfs(0, A, mask, 0)) return 0;
    } while (next_permutation(A.begin(), A.end()));
    return 1;
}

int getCost(int mask) {
    int res = 0;
    for (int i = 0; i < n * n; i++) {
        if (!getBit(mask, i)) continue;
        int who = i / n, task = i % n;
        if (s[who][task] == '0') res++;
    }
    return res;
}

void solve() {
    int m = n * n;
    int maxMask = (1 << m) - 1, worst = 1e9;
    for (int mask = 0; mask <= maxMask; mask++) {
        if (mask == 8) {
            int t = 0;
        }
        if (ok(mask)) {
            int cur = getCost(mask);
            worst = min(worst, cur);
        }
    }
    cout << worst << endl;
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    
    int T; cin >> T;
    for (int te = 1; te <= T; te++) {
        cin >> n;
        for (int i = 0; i < n; i++) cin >> s[i];
        cout << "Case #" << te << ": ";
        solve();
    }
    
    return 0;
}
