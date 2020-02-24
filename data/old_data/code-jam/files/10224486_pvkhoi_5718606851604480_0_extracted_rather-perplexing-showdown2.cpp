#include <bits/stdc++.h>

using namespace std;

#define sz(a) int(a.size())

char G[3] = {'R', 'S', 'P'};

string res;
int a[10000];
int n, R, P, S;

string fight(string s) {
    string res(sz(s) / 2, ' ');
    for (int i = 0; i < sz(s); i += 2) {
        if (s[i] == s[i+1]) return s;
        if (s[i] == 'R') {
            if (s[i+1] == 'P') res[i/2] = 'P';
            else res[i/2] = 'R';
        }
        else if (s[i] == 'P') {
            if (s[i+1] == 'R') res[i/2] = 'P';
            else res[i/2] = 'S';
        }
        else {
            if (s[i+1] == 'P') res[i/2] = 'S';
            else res[i/2] = 'R';
        }
    }
    return res;
}

void solve() {
    res = "";
    string cur = "";
    for (int i = 0; i < P; i++) cur += "P";
    for (int i = 0; i < R; i++) cur += "R";
    for (int i = 0; i < S; i++) cur += "S";
    do {
        string tmp = cur;
        while (sz(tmp) > 1) {
            string next = fight(tmp);
            if (next == tmp) break;
            tmp = next;
        }
        if (sz(tmp) == 1) {
            if (res == "" || cur < res) res = cur;
        }
    } while (next_permutation(cur.begin(), cur.end()));
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

