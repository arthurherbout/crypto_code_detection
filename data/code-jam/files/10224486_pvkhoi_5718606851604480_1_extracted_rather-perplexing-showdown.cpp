#include <bits/stdc++.h>

using namespace std;

#define sz(a) int(a.size())

char G[3] = {'R', 'S', 'P'};

string res;
int a[10000];
int n, R, P, S;

bool isLastLayer(int x) {
    return x >= (1 << (n+1)) - (1 << n);
}

void dfs(int p, int x) {
    a[p] = x;
    if (isLastLayer(p)) return;
    if (x == 0) {
        dfs(p*2, 0), dfs(p*2+1, 1);
    }
    else if (x == 1) dfs(p*2, 2), dfs(p*2+1, 1);
    else dfs(p*2, 2), dfs(p*2+1, 0);
}

bool ok(string s) {
    vector <int> cnt(3, 0);
    for (int i = 0; i < sz(s); i++)
        if (s[i] == 'R') cnt[0]++;
        else if (s[i] == 'P') cnt[2]++;
        else cnt[1]++;
    return cnt[0] == R && cnt[1] == S && cnt[2] == P;
}

string optimize(string s) {
    string res = s;
    for (int x = 1; x < (1 << n); x <<= 1) {
        for (int i = 0; i < sz(res); i += x*2) {
            string A = res.substr(i, x), B = res.substr(i+x, x);
            if (B < A) {
                string C = B + A;
                for (int j = 0; j < sz(C); j++)
                    res[i+j] = C[j];
            }
        }
    }
    return res;
}

void thu(int x) {
    memset(a, -1, sizeof a);
    dfs(1, x);
    string cur = "";
    for (int i = (1 << (n+1)) - (1 << n); i < (1 << (n+1)); i++) {
        if (a[i] == -1) return;
        cur += G[a[i]];
    }
    if (ok(cur)) {
        string best = optimize(cur);
        if (res == "" || best < res) res = best;
    }
}

void solve() {
    res = "";
    thu(0);
    thu(1);
    thu(2);
    if (res == "") cout << "IMPOSSIBLE\n";
    else cout << res << endl;
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    
    ios::sync_with_stdio(0);
    int T; cin >> T;
    for (int te = 1; te <= T; te++) {
        cin >> n >> R >> P >> S;
        cout << "Case #" << te << ": ";
        solve();
        
    }
    
    return 0;
}
