#include <bits/stdc++.h>
#define ll long long
using namespace std;

#define V 1001111
#define N 51111
int ct[V];
int dice[N][6];
vector<int> mern[V];
int n;
int front[V];
int back[N];
int vis[V];
int typ;

bool dfs(int i) {
    assert(vis[i] != typ);
    vis[i] = typ;
    for (int j : mern[i]) {
        if (back[j] == -1 || (vis[back[j]] != typ && dfs(back[j]))) {
            back[j] = i;
            front[i] = j;
            return true;
        }
    }
    return false;
}

int match;
bool good(int I, int J) {
    int m = J - I + 1;
    assert(m >= 0);
    if (m == 0) return true;
    if (ct[J] - ct[I - 1] != m) return false;
    typ++;
    if (dfs(J)) match++;
    return match == m;
}

void solve(int cas) {
    srand(cas);
    printf("Case #%d:", cas);
    for (int i = 0; i < V; i++) ct[i] = 0;
    typ = 0;
    for (int i = 0; i < V; i++) {
        vis[i] = -1;
        mern[i].clear();
    }
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < 6; j++) {
            scanf("%d", &dice[i][j]);
            mern[dice[i][j]].push_back(i);
            ct[dice[i][j]] = 1;
        }
    }
    for (int i = 1; i < V; i++) {
        ct[i] += ct[i - 1];
    }

    for (int i = 0; i < V; i++) {
        front[i] = -1;
        random_shuffle(mern[i].begin(), mern[i].end());
    }
    for (int i = 0; i < n; i++) back[i] = -1;
    match = 0;

    int best = 1;
    for (int L = 1, R = 0; L < V && R < V; L++) {
        if (front[L-1] != -1) {
            back[front[L-1]] = -1;
            front[L-1] = -1;
            match--;
        }
        assert(R - L + 1 >= 0);
        while (R < V && good(L, R)) R++;
        best = max(best, R - L);
    }

    printf(" %d", best);
    puts("");
}
int main() {
    int z;
    scanf("%d", &z);
    fprintf(stderr, "z = %d\n", z);
    for (int cas = 1; cas <= z; cas++) {
        solve(cas);
    }
}
