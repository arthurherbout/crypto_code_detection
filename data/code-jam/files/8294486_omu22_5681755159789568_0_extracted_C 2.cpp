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

#define int long long
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
const int INF = 1e17 + 9;

signed main() {
	cin.tie(0);
	ios_base::sync_with_stdio(false);

	int T;
	cin >> T;

	rep(_, T) {
		cout << "Case #" << _ + 1 << ": ";
		int n, q;
		cin >> n >> q;
		vector<int> e(n), s(n);
		for (int i = 0; i < n; i++) {
			cin >> e[i] >> s[i];
		}
		int d[101][101];
		rep(i, n)rep(j, n) {
			cin >> d[i][j];
		}
		rep(i, q) {
			int u, v;
			cin >> u >> v;
			u--; v--;

			double dp[101];
			rep(j, n)dp[j] = INF;
			dp[0] = 0;

			rep(j, n) {				
				double ds = e[j];
				double sm = 0;
				for (int k = j + 1; k < n; k++) {
					sm += d[k - 1][k];
					ds -= d[k - 1][k];
					if (ds >= 0) {
						dp[k] = min(dp[k], dp[j] + sm / s[j]);
					}
				}
			}

			cout << setprecision(30) << fixed << dp[n - 1] << endl;


		}

	}

	return 0;
}