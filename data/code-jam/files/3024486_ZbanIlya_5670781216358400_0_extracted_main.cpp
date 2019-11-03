#include <iostream>
#include <string>
#include <map>
#include <math.h>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <set>
#include <queue>
#include <sstream>
#include <deque>
#include <memory.h>
#include <cassert>
#include <ctime>


using namespace std;

#define ll long long
#define y1 _dfdfdfd


const int maxn = 100;
const int inf = 1000000007;
const int mod = 1000000007;

int n;
int c[maxn];
vector<int> e[maxn];
//map<pair<int, int>, bool> mp;
char mp[maxn][maxn];

int rec(int ci, int cj, int k, int cur, bool fi, bool fj) {
    if (!fi && !fj) return cur;
    if (k == 0) {
        int res = -inf;
        cur += c[ci];
        int o = c[ci];
        c[ci] = 0;
        for (int j = 0; j < e[ci].size(); j++) {
            if (!mp[ci][e[ci][j]]) {
                mp[ci][e[ci][j]] = mp[e[ci][j]][ci] = 1;
                res = max(res, rec(e[ci][j], cj, k ^ 1, cur, fi, fj));
                mp[ci][e[ci][j]] = mp[e[ci][j]][ci] = 0;
            }
        }
        if (res == -inf) res = rec(ci, cj, k ^ 1, cur, 0, fj);
        c[ci] = o;
        return res;
    } else {
        int res = inf;
        cur -= c[cj];
        int o = c[cj];
        c[cj] = 0;
        for (int j = 0; j < e[cj].size(); j++) {
            if (!mp[make_pair(cj, e[cj][j])]) {
                mp[make_pair(cj, e[cj][j])] = mp[make_pair(e[cj][j], cj)] = 1;
                res = min(res, rec(ci, e[cj][j], k ^ 1, cur, fi, fj));
                mp[make_pair(cj, e[cj][j])] = mp[make_pair(e[cj][j], cj)] = 0;
            }
        }
        if (res == inf) res = rec(ci, cj, k ^ 1, cur, fi, 0);
        c[cj] = o;
        return res;        
    }
}
 
int main() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

    int tests;
    cin >> tests;
    for (int test = 1; test <= tests; test++) {
        cerr << "Case #" << test << ": ";
        cout << "Case #" << test << ": ";

        cin >> n;
        for (int i = 1; i <= n; i++) {
            e[i].clear();
            cin >> c[i];
        }
        mp.clear();
        for (int i = 1; i < n; i++) {
            int u, v;
            cin >> v;
            u = i;
            e[u].push_back(v);
            e[v].push_back(u);
            mp[make_pair(u, v)] = mp[make_pair(v, u)] = 0;
        }
        int ans = -inf;
        for (int i = 1; i <= n; i++) {
            int res = inf;
            for (int j = 1; j <= n; j++) {
                res = min(res, rec(i, j, 0, 0, 1, 1));
            }
            ans = max(ans, res);
        }
        cout << ans << endl;
        cerr << ans << endl;
    }

	return 0;
}
