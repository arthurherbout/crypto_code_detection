#include <cstdio>
#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <string>
#include <cstring>
#include <sstream>
#include <algorithm>
#include <functional>
#include <queue>
#include <stack>
#include <cmath>
#include <iomanip>
#include <list>
#include <tuple>
#include <bitset>
#include <ciso646>
#include <cassert>

using namespace std;

#define double long double

typedef pair<int, int> P;
typedef tuple<int, int, int> T;

template<class T> string tostr(T x) { stringstream o; o << x; return o.str(); }
template<class T> T sqr(T x) { return x*x; }
template<class T> T mypow(T x, int n) { T r = 1; while (n > 0) { if (n & 1)r = r*x; x = x*x; n >>= 1; }return r; }

int toint(string s) { int v; stringstream i(s); i >> v; return v; }
bool check(int x, int y, int w, int h) { return x >= 0 && y >= 0 && w > x && h > y; }
int gcd(int a, int b) { return b ? gcd(b, a%b) : a; }
int lcm(int a, int b) { return a / gcd(a, b) * b; }

#define REP(i,a,b)	for(int (i) = (a);i < (b);(i)++)
#define rep(i,n)	REP(i,0,n)
#define PER(i,a,b)	for(int (i) = (a-1);i >= (b);(i)--)
#define per(i,n)	PER(i,n,0)
#define each(i,n)	for(auto &i : n)
#define clr(a)		memset((a), 0 ,sizeof(a))
#define mclr(a)		memset((a), -1 ,sizeof(a))
#define dump(val) 	cerr << #val " = " << val << endl;
#define dum(val)	cerr << #val " = " << val;
#define FILL(a,v)	fill(a,a+sizeof(a)/sizeof(*a),v)

const int dx[8] = { +1,+0,-1,+0,+1,+1,-1,-1 };
const int dy[8] = { +0,-1,+0,+1,-1,+1,-1,+1 };
const int mod = 1e9 + 7;
const int INF = 1e9 + 9;

struct edge { int to, cap, rev; };
vector<edge> G[20];
bool used[20];

int lg[20][20];
void clear() {
	rep(i, 20) {
		G[i].clear();
		used[i] = false;
	}
	clr(lg);
}
void add_edge(int from, int to, int cap) {
	G[from].push_back(edge{ to, cap, int(G[to].size()) });
	G[to].push_back(edge{ from, 0, int(G[from].size()) - 1 });
}


int dfs(int v, int t, int f) {
	if (v == t)return f;
	used[v] = true;
	rep(i, G[v].size()) {
		edge &e = G[v][i];
		if (!used[e.to] && e.cap > 0) {
			int d = dfs(e.to, t, min(f, e.cap));
			if (d > 0) {
				e.cap -= d;
				lg[v][e.to] += d;
				G[e.to][e.rev].cap += d;
				lg[e.to][v] -= d;
				return d;
			}
		}
	}
	return 0;
}
int max_flow(int s, int t) {
	int flow = 0;
	for (;;) {
		clr(used);
		int f = dfs(s, t, INF);
		if (f == 0)return flow;
		flow += f;
	}
}

signed main() {
	cin.tie(0);
	ios_base::sync_with_stdio(false);

	int T;
	cin >> T;

	rep(_, T) {
		cout << "Case #" << _ + 1 << ": ";

		clear();

		int n;
		cin >> n;
		int t[6];
		rep(i, 6)cin >> t[i];

		int d[6][6];
		clr(d);


		rep(i, 6) {
			rep(j, 6) {
				if (i != j && (i + 1) % 6 != j && (j + 1) % 6 != i) {
					d[i][j] = true;
				}
			}
		}
		int S = 15, E = 16;

		rep(i, 6) {
			add_edge(S, i, t[i]);
			add_edge(i + 6, E, t[i]);
		}
		rep(i, 6) {
			rep(j, 6) {
				if (d[i][j]) {
					add_edge(i, j + 6, t[j]);
				}
			}
		}

		int MAX = max_flow(S, E);

		if (MAX != n) {
			cout << "IMPOSSIBLE" << endl;
		}
		else {

			string ans;

			string s = "ROYGBV";

			int st = 0;
			while (ans.size() < n) {
				P ma(-1, -1);
				for(int j = 6;j < 12; j++) {
					ma = max(ma, P(lg[st][j], j));
				}
				lg[st][ma.second]--;
				st = ma.second - 6;
				ans.push_back(s[ma.second - 6]);
			}

			cout << ans << endl;
		}

	}

	return 0;
}