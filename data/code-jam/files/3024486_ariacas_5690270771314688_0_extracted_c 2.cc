#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <iostream>
#include <set>
#include <vector>
#include <cstring>
#include <string>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <map>
using namespace std;
typedef long long ll;
typedef vector<int> vi;
typedef vector<ll> vl;
typedef vector<vi> vvi;
typedef vector<vl> vvl;
typedef vector<double> vd;
typedef pair<int, int> pii;
typedef pair<double, double> pdd;
typedef vector<pii> vii;
typedef vector<string> vs;

vvi g;
vector<int> mt;
vector<char> used;
vi bad;
 
bool try_kuhn (int v) {
	if (used[v])  return false;
	used[v] = true;
    //if (mt[v] != -1) return false;
	for (size_t i=0; i<g[v].size(); ++i) {
		int to = g[v][i];
		if (mt[to] == -1 || try_kuhn (mt[to])) {
			mt[to] = v;
			mt[v] = to;
			return true;
		}
	}
	return false;
}

int main() {
    int T;
    cin >> T;
    string crime = "CRIME TIME\n";
    for (int test = 1; test <= T; ++test) {
        printf("Case #%d: ", test);
        int n;
        cin >> n;
        vi in(n), x(n);
        int maxind = 0;
        for (int i = 0; i < n; ++i) {
            string s;
            cin >> s >> x[i];
            if (s[0] == 'E') in[i] = 1;
            maxind = max(maxind, x[i]);
        }
        g.clear();
        g.resize(n);
        mt.assign (n, -1);
        bad.assign (n, 0);
        vi lastenter(maxind+1);
        vi firstout(maxind+1);
        for (int i = 0; i < n; ++i) {
            if (in[i]) lastenter[x[i]] = i;
        }
        for (int i = n-1; i >= 0; --i) {
            if (in[i] == 0) firstout[x[i]] = i;
        }
        for (int i = 0; i < n; ++i) {
            if (in[i] && lastenter[x[i]] != i && x[i]) {
                bad[i] = 1;
                for (int j = i + 1; j < n; ++j) if (!in[j] && (!x[j] || x[j] == x[i])) {
                    g[i].push_back(j);
                    g[j].push_back(i);
                }
            }
            if (!in[i] && firstout[x[i]] != i && x[i]) {
                bad[i] = 1;
                for (int j = 0; j < i; ++j) if (in[j] && (!x[j] || x[j] == x[i])) {
                    g[i].push_back(j);
                    g[j].push_back(i);
                }
            }
        }
        bool fail = 0;
        for (int v=0; v<n; ++v) if (bad[v] && mt[v] == -1) {
//            cerr << test << ' ' << v << ' ' << g[v].size() << endl;
            used.assign (n, false);
            try_kuhn (v);
            if (mt[v] == -1) {
                fail = 1;
                break;
            }
        }
        if (fail) {
            cout << crime;
            continue;
        }
        for (int i = 0; i < n; ++i) {
            if (in[i]) {
                for (int j = i + 1; j < n; ++j) if (!in[j] && (!x[j] || x[j] == x[i])) {
                    g[i].push_back(j);
                    g[j].push_back(i);
                }
            }
            if (!in[i]) {
                for (int j = 0; j < i; ++j) if (in[j] && (!x[j] || x[j] == x[i])) {
                    g[i].push_back(j);
                    g[j].push_back(i);
                }
            }
        }
        for (int v=0; v<n; ++v) if (mt[v] == -1) {
            used.assign (n, false);
            try_kuhn (v);
        }
        int ans = 0;
        for (int i = 0; i < n; ++i) if (in[i] && mt[i] == -1) ++ans;
        cout << ans << endl;
    }
    return 0;
}
