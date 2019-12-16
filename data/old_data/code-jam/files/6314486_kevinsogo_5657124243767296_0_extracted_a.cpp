#include <bits/stdc++.h>
#define ll long long
using namespace std;

#define V 1001111
#define N 51111
int ct[V];
int dice[N][6];
int n;
int front[N];
int back[V];
bool vis[V];
int I, J;

bool dfs(int i) {
    assert(!vis[i]);
    vis[i] = true;
    for (int k = 0; k < 6; k++) {
        int j = dice[i][k];
        if (!(I <= j && j <= J)) continue;
        if (back[j] == -1 || (!vis[back[j]] && dfs(back[j]))) {
            back[j] = i;
            front[i] = j;
            return true;
        }
    }
    return false;
}

bool good(int I, int J, int m) {
    ::I = I;
    ::J = J;
    for (int i = 0; i < n; i++) front[i] = -1;
    for (int i = I; i <= J; i++) back[i] = -1;
    int match = 0;
    for (int i = 0; i < n; i++) {
        if (front[i] == -1) {
            for (int j = 0; j < n; j++) vis[j] = false;
            if (dfs(i)) match++;
        }
    }
    return match == m;
}
bool can(int m) {
    for (int I = 1, J = m; J < V; I++, J++) {
        if (ct[J] - ct[I - 1] == m) {
            if (good(I, J, m)) return true;
        }
    }
    return false;
}

void solve(int cas) {
    printf("Case #%d:", cas);
    for (int i = 0; i < V; i++) ct[i] = 0;
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < 6; j++) {
            scanf("%d", &dice[i][j]);
            ct[dice[i][j]] = 1;
        }
    }
    for (int i = 1; i < V; i++) {
        ct[i] += ct[i - 1];
    }

    int L = 1, R = n + 1;
    while (R - L > 1) {
        int M = L + R >> 1;
        (can(M) ? L : R) = M;
    }
    printf(" %d", L);
    puts("");
}
int main() {
    int z;
    scanf("%d", &z);
    for (int cas = 1; cas <= z; cas++) {
        solve(cas);
    }
}
