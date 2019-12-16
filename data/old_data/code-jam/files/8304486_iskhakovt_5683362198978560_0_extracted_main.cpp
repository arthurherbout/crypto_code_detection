#define _USE_MATH_DEFINES

#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <climits>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <deque>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <utility>
#include <vector>

using namespace std;

typedef long long ll;
typedef long double ld;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;

const int INF = 1000 * 1000 * 1000 + 11;
const ll LINF = (ll)INF * INF;

struct Pos {
    ll l_limit, u_limit;
    int x, y;
    
    Pos() {}
    Pos(ll l_limit, ll u_limit, int x, int y) : l_limit(l_limit), u_limit(u_limit), x(x), y(y) {}
    
    bool operator<(const Pos &p) const {
        if (u_limit == p.u_limit) {
            if (l_limit == p.l_limit) {
                if (x == p.x) return y < p.y;
                return x < p.x;
            }
            return l_limit < p.l_limit;
        }
        return u_limit < p.u_limit;
    }
};


bool correct(int x, int y, int n, int m) {
    return 0 <= x && x < n && 0 <= y && y < m;
}

const vector<pair<int, int>> ds = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

ll const MOD = 1000 * 1000 * 1000 + 7;

ll solve(int n, int m, ll D, vector<Pos> poses_list) {
    set <Pos> poses;
    vector<vector<ll>> l_limit(n, vector<ll>(m, 0));
    vector<vector<ll>> u_limit(n, vector<ll>(m, LINF));
    vector<vector<ll>> result(n, vector<ll>(m, LINF));
    
    for (auto const &pos : poses_list) {
        l_limit[pos.x][pos.y] = pos.l_limit;
        u_limit[pos.x][pos.y] = pos.u_limit;
        poses.insert(pos);
    }
    
    vector<vector<bool>> vis(n, vector<bool>(m, false));
    
    while (!poses.empty()) {
        Pos pos = *poses.begin();
        poses.erase(poses.begin());
        
        vis[pos.x][pos.y] = true;
        assert(pos.u_limit != LINF);
        
        result[pos.x][pos.y] = pos.u_limit;
        
        /*
        if (pos.x == 9) {
            cout << "!!!\n";
        }
         */
        
        for (const auto &d : ds) {
            int x = pos.x + d.first, y = pos.y + d.second;
            
            /*
            if (x == 11) {
                cout << "?!\n";
            }
            
             */
            //cout << x << ' '<< y << '\n';
            
            
            if (!correct(x, y, n, m)) continue;
            if (result[x][y] != LINF) continue;
            
            poses.erase(Pos(l_limit[x][y], u_limit[x][y], x, y));
            
            l_limit[x][y] = max(l_limit[x][y], result[pos.x][pos.y] - D);
            u_limit[x][y] = min(u_limit[x][y], result[pos.x][pos.y] + D);
            
            assert(u_limit[x][y] != LINF);
            
            if (l_limit[x][y] > u_limit[x][y]) {
                return -1;
            }
            poses.insert(Pos(l_limit[x][y], u_limit[x][y], x, y));
        }
    }
    
    /*
    for (int i = 0; i != n; ++i) {
        for (int j = 0; j != m; ++j) {
            cout << result[i][j] << ' ';
        }
        cout << '\n';
    }
    */
    
    ll sum = 0;
    for (int i = 0; i != n; ++i) {
        for (int j = 0; j != m; ++j) {
            assert(vis[i][j]);
            assert(result[i][j] != LINF);
            sum = (sum + result[i][j]) % MOD;
        }
    }
    
    return sum;
}

#include <cstdio>

int main() {
    //freopen("/Users/iskhakovt/Downloads/D-small-attempt0.in", "r", stdin);
    
    ios_base::sync_with_stdio(false);
    
    int tests;
    cin >> tests;
    
    for (int test = 0; test != tests; ++test) {
        cout << "Case #" << test + 1 << ": ";
        
        int n, m, N;
        ll D;
        cin >> n >> m >> N >> D;
        
        vector<Pos> poses(N);
        for (auto &pos : poses) {
            cin >> pos.x >> pos.y >> pos.u_limit;
            --pos.x; --pos.y; pos.l_limit = pos.u_limit;
        }
        
        
        ll sol = solve(n, m, D, poses);
        if (sol == -1) {
            cout << "IMPOSSIBLE\n";
        } else {
            cout << sol << "\n";
        }
    }
    
    return 0;
}