#include "bits/stdc++.h"
using namespace std;

const int C = 31;
#define int long long
#define f first
#define s second
#define ppi pair < pair < int, int > , int >

int c;
int e[C], l[C], d[C];
vector < int > adj[C];
map < int, int > distances;
map < ppi, int > dp;

inline int get_cost(int x, int y) {
    // x -> y cost
    if (x <= y) {
        return y - x;
    } else {
        return (24 - x) + (y);
    }
}

inline int solve(int last, int hour, int mask) {
    ppi res = make_pair(make_pair(last, hour), mask);
    if (mask == (1 << (2 * c)) - 1) {
        return dp[res] = 0;
    }
    if (dp.count(res)) {
        return dp[res];
    }
    int sol = INT_MAX;
    dp[res] = 0;
    for (int x : adj[last]) {
        x--;
        if (mask & (1 << x)) {
            continue;
        }
        int _last = e[x + 1];
        int _hour = (l[x + 1] + d[x + 1]) % 24;
        int _mask = mask | (1 << x);
        int cost = get_cost(hour, l[x + 1]);
        sol = min(sol, cost + d[x + 1] + solve(_last, _hour, _mask));
    }
    return dp[res] = sol;
}

#undef int
int main() {
    #define int long long
    freopen("inp.in", "r", stdin);
    freopen("inp.out", "w", stdout);
    int tests; cin >> tests;
    for (int qq = 1; qq <= tests; qq++) {
        cout << "Case #" << qq << ": ";
        cin >> c;
        dp.clear();
        for (int i = 1; i <= c; i++) {
            adj[i].clear();
            e[i] = l[i] = d[i] = 0;
        }
        distances.clear();
        for (int i = 1; i <= 2 * c; i++) {
            cin >> e[i] >> l[i] >> d[i];
            int s = (i + 1) / 2;
            adj[s].push_back(i);
        }
        cout << solve(1, 0, 0) << endl;
        /*distances[0] = 0;
        priority_queue < pair < int, int > > dijkstra;
        dijkstra.push({0, 0});
        while (!dijkstra.empty()) {
            pair < int, int > cur = dijkstra.top();
            dijkstra.pop();
            int d = cur.first;
            int mask = cur.second;
            if (distances[mask] < d) {
                continue;
            }
            for ()
        }*/
    }
}
