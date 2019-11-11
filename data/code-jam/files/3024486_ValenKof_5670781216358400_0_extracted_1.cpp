#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <iomanip>
#include <bitset>
#include <memory>

using namespace std;
 
#define debug(x) cerr << "DEBUG: " << #x << " = " << x << endl
#define forn(i, n) for(int i = 0; i < (n); ++i)
#define all(x) x.begin(), x.end()
#define mp make_pair
#define pb push_back

template <typename T> inline void mn(T& x, const T& y) { if (y < x) x = y; }
template <typename T> inline void mx(T& x, const T& y) { if (x < y) x = y; }
template <typename T> inline int sz(const T& x) { return (int) x.size(); }

const int N = 80;

int money[N];
int visitedEdge[3];
int visitedNode[3];
vector<pair<int, int> > g[N];

struct Node {
    int usedNode[3];
    int usedEdge[3];

    Node() {
        fill(usedEdge, usedEdge + 3, 0);
        fill(usedNode, usedNode + 3, 0);
    }

    inline bool UsedEdge(int i) const {
        return usedEdge[i >> 5] & (1 << (i & 31));
    }
    inline bool UsedNode(int i) const {
        return usedNode[i >> 5] & (1 << (i & 31));
    }

    inline void fillDfs(int u) {
        visitedNode[u >> 5] |= (1 << (u & 31));
        for (auto arc : g[u]) {
            if (UsedEdge(arc.second)) {
                continue;
            }
            int i = arc.second;
            int v = arc.first;
            if (visitedEdge[i >> 5] & (1 << (i & 31))) {
                continue;
            }
            visitedEdge[i >> 5] |= (1 << (i & 31));
            fillDfs(v);
        }
    }

};

inline bool operator<(const Node& a, const Node& b) {
    return memcmp(&a, &b, sizeof(Node)) < 0;
}

map<Node, int> dp[N][N];

int n;

inline bool canMove(const Node& state, int u) {
    for (auto p : g[u]) {
        if (!state.UsedEdge(p.second)) {
            return true;
        }
    }
    return false;
}

int maskNode[3];
int maskEdge[3];

inline int DP(Node state, int p1, int p2, bool moved) {

    // cout << p1 << ' ' << p2 << endl;

    fill(visitedNode, visitedNode + 3, 0);
    fill(visitedEdge, visitedEdge + 3, 0);

    state.fillDfs(p1);
    state.fillDfs(p2);

    forn (i, 3) {
        state.usedEdge[i] |= (~visitedEdge[i]) & maskEdge[i];
        state.usedNode[i] |= (~visitedNode[i]) & maskNode[i];
    }

    auto it = dp[p1][p2].find(state);
    if (it != dp[p1][p2].end()) {
        return it->second;
    }


    int result = numeric_limits<int>::max();
    int add = 0;

    int& dest = dp[p1][p2][state] = 0;

    if (!state.UsedNode(p1)) {
        state.usedNode[p1 >> 5] |= 1 << (p1 & 31);
        add += money[p1];
    }

    if (!canMove(state, p1)) {
        if (!moved) {
            return dest = 0;
        }
        result = DP(state, p2, p1, true);
    } else {
        for (auto arc : g[p1]) {
            if (state.UsedEdge(arc.second)) {
                continue;
            }
            int i = arc.second;
            state.usedEdge[i >> 5] ^= 1 << (i & 31);
            mn(result, DP(state, p2, arc.first, true));
            state.usedEdge[i >> 5] ^= 1 << (i & 31);
        }
    }
    return dest = add - result;
}


int main() {
    // freopen("in.txt", "r", stdin);
    freopen("D-small-attempt1.in.txt", "r", stdin);
    freopen("out.txt", "w", stdout);

    ios_base::sync_with_stdio(false);

    int nTests;
    cin >> nTests;
    forn (iTest, nTests) {
        cin >> n;
        // debug(n);
        cout << "Case #" << (iTest + 1) << ": ";

        forn (i, 3) {
            maskNode[i] = 0;
            maskEdge[i] = 0;
        }
        forn (i, n) {
            maskNode[i >> 5] |= 1 << (i & 31);
        }
        forn (i, n - 1) {
            maskEdge[i >> 5] |= 1 << (i & 31);
        }

        forn (i, n) {
            cin >> money[i];
            g[i].resize(0);
        }

        forn (i, n - 1) {
            int j;
            cin >> j;
            --j;
            g[i].pb(mp(j, i));
            g[j].pb(mp(i, i));
        }

        forn (i, n) forn (j, n) {
            dp[i][j].clear();
        }

        Node init;

        int best = numeric_limits<int>::min();
        forn (i, n) {
            int bestOther = numeric_limits<int>::max();
            forn (j, n) {
                mn(bestOther, DP(init, i, j, true));                
            }
            mx(best, bestOther);
        }
        cout << best << endl;

    }    

    return 0;
}