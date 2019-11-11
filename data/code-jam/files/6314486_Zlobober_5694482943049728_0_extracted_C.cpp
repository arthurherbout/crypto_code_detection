#include <cstdio>
#include <cassert>
#include <algorithm>
#include <set>
#include <tuple>
using namespace std;

const int N = 22;
const int MX = 10000;

int cnt = 0;
bool was[MX + 1];
struct graph {
    vector<vector<int>> E;
    int s, t;
    void add_edge(int a, int b) {
        if (a < b)
            swap(a, b);
        if (a >= E.size()) {
            E.resize(a + 1);
        }
        E[a].push_back(b);
        E[b].push_back(a);
    }
};
graph G[MX + 1];

set<tuple<int, int, int, int>> cache;

graph basic() {
    graph g;
    g.s = 0;
    g.t = 1;
    g.E = {{1}, {0}};
    return g;
}

graph ext_par(graph g) {
    g.E[g.s].push_back(g.t);
    g.E[g.t].push_back(g.s);
    return g;
}

graph ext_par2(graph g) {
    int v = g.E.size();
    int s = g.s;
    int t = g.t;
    auto& E = g.E;
    E.emplace_back();
    E[s].push_back(v);
    E[v].push_back(s);
    E[t].push_back(v);
    E[v].push_back(t);
    return g;
}

graph ext_cycle(graph g, int l) {
    vector<int> vers;
    for (int i = 0; i < l; i++) {
        vers.push_back(g.E.size() + i);
    }
    g.add_edge(g.t, vers[0]);
    for (int i = 0; i < l; i++) {
        g.add_edge(vers[i], vers[(i + 1) % l]);
    }
    return g;
}

graph ext_park(graph g) {
    int v = g.E.size();
    g.E.emplace_back();
    int u = g.E.size();
    int s = g.s;
    int t = g.t;
    auto& E = g.E;
    E.emplace_back();
    E[s].push_back(v);
    E[v].push_back(s);
    E[v].push_back(u);
    E[u].push_back(v);
    E[t].push_back(u);
    E[u].push_back(t);
    return g;
}

graph ext_pos(graph g) {
    int v = g.E.size();
    auto& E = g.E;
    int t = g.t;
    E.emplace_back();
    E[t].push_back(v);
    E[v].push_back(t);
    g.t = v;
    return g;
}

vector<graph> stk = {basic()};

void go(int n, pair<int, int> st, bool kor) {
    if (!cache.insert(make_tuple(n, st.first, st.second, (int)kor)).second) {
        return;
    }
    if (n > N) {
        return;
    }
    if (st.first > MX) {
        return;
    }
    if (!was[st.first]) {
        was[st.first] = true;
        fprintf(stderr, "! %d\n", st.first);
        ++cnt;
        fprintf(stderr, "# %d\n", cnt);
        G[st.first] = stk.back();
    }
    if (!kor) {
        stk.emplace_back(ext_par(stk.back()));
        go(n, {st.first + st.second, st.second}, true);
        stk.pop_back();
    }
    stk.emplace_back(ext_par2(stk.back()));  
    go(n + 1, {st.first * 2 + st.second, 2 * st.second}, kor);
    stk.pop_back();
    /*stk.emplace_back(ext_par3(stk.back()));  
    go(n + 2, {st.first * 3 + st.second, 3 * st.second}, kor);
    stk.pop_back();*/
    stk.emplace_back(ext_pos(stk.back()));
    go(n + 1, {st.first, st.second + st.first}, false);
    stk.pop_back();
}

void process_hand() {
    // 25 33 42 70
    G[25] = ext_cycle(ext_cycle(basic(), 5), 5);
    G[33] = ext_cycle(ext_cycle(basic(), 3), 11);
    G[42] = ext_cycle(ext_cycle(basic(), 6), 7);
    G[70] = ext_cycle(ext_cycle(basic(), 10), 7);
    was[25] = was[33] = was[42] = was[70] = true;
}

int main() {
    go(2, {1, 1}, true);
    process_hand();
    int T;
    scanf("%d", &T);
    for (int cs = 1; cs <= T; cs++) {
        int k;
        scanf("%d", &k);
//    for (int k = 3; k <= MX; k++) {
        //printf("k = %d\n", k);
        if (!was[k]) {
            assert(false);
            //printf("FAIL\n");
            //continue;
        }
        int n = G[k].E.size();
        printf("Case #%d: %d\n", cs, n);
        assert(2 <= n && n <= 22);

        vector<vector<char>> adj(n, vector<char>(n, '0'));
        for (int i = 0; i < n; i++) {
            for (int j : G[k].E[i]) {
                if (i > j)
                    continue;
                assert(adj[i][j] == '0');
                adj[i][j] = adj[j][i] = '1';
                assert(i != j);
            }
        }
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                printf("%c", adj[i][j]);
            }
            printf("\n");
        }
        /*
            sort(G[k].E[i].begin(), G[k].E[i].end());
            for (int j : G[k].E[i]) {
                assert(i != j);
                if (j > i) {
                    printf("%d %d\n", i, j); 
                }
            }
        }*/
        fflush(stdout);
    }
}
