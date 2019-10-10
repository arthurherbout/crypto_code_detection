#include <bits/stdc++.h>

using namespace std;

#define sz(a) int(a.size())

string C, J;

void solve() {
    int n = sz(C);
    
    int i;
    for (i = 0; i < n; i++) {
        if (C[i] != '?' && J[i] != '?' && C[i] != J[i]) break;
        if (C[i] != '?') J[i] = C[i];
        else if (J[i] != '?') C[i] = J[i];
        else {
            C[i] = J[i] = '0';
        }
    }
    
    int si;
    if (C[i] < J[i]) si = -1;
    else si = 1;
    
    if (si == -1) {
        for (int j = i; j < n; j++) {
            if (C[j] == '?') C[j] = '9';
            if (J[j] == '?') J[j] = '0';
        }
    }
    else {
        for (int j = i; j < n; j++) {
            if (C[j] == '?') C[j] = '0';
            if (J[j] == '?') J[j] = '9';
        }
    }
    
    cout << C << " " << J << endl;
}

string toString(int x) {
    if (x == 0) return "0";
    string res = "";
    while (x) {
        res = char(x % 10 + '0') + res;
        x /= 10;
    }
    return res;
}

string dep(string a, int n) {
    while (sz(a) < n) a = "0" + a;
    return a;
}

bool ok(string a, string b) {
    int n = sz(b);
    if (sz(a) > n) return 0;
    a = dep(a, n);
    for (int i = 0; i < n; i++) {
        if (b[i] != '?' && a[i] != b[i]) return 0;
    }
    return 1;
}

void trau() {
    int res = 1e9, opC = 1e9, opJ = 1e9;
    for (int X = 0; X < 1000; X++) {
        string t = toString(X);
        if (ok(t, C)) {
            for (int Y = 0; Y < 1000; Y++) {
                string p = toString(Y);
                if (ok(p, J)) {
                    if (X == 19 && Y == 20) {
                        int tt = 0;
                    }
                    int cur = abs(X - Y);
                    if (cur < res || (cur == res && X < opC) || (cur == res && X == opC && Y < opJ)) {
                        res = cur;
                        opC = X;
                        opJ = Y;
                    }
                }
            }
        }
    }
    cout << dep(toString(opC), sz(C)) << " " << dep(toString(opJ), sz(C)) << endl;
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    
    int T; cin >> T;
    for (int te = 1; te <= T; te++) {
        cin >> C >> J;
        cout << "Case #" << te << ": ";
//        solve();
        trau();
    }
    
    return 0;
}
