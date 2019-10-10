#include <iostream>
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include <bitset>
#include <queue>
#include <stack>
#include <sstream>
#include <cstring>
#include <numeric>
#include <ctime>

#define re return
#define fi first
#define se second
#define mp make_pair
#define pb push_back
#define all(x) (x).begin(), (x).end()
#define sz(x) ((int) (x).size())
#define rep(i, n) for (int i = 0; i < (n); i++)
#define rrep(i, n) for (int i = (n) - 1; i >= 0; i--)
#define y0 y32479
#define y1 y95874
#define fill(x, y) memset(x, y, sizeof(x))
#define sqr(x) ((x) * (x))
#define sqrt(x) sqrt(abs(x))
#define unq(x) (x.resize(unique(all(x)) - x.begin()))
#define spc(i,n) " \n"[i == n - 1]

using namespace std;

typedef vector<int> vi;
typedef vector<vi> vvi;
typedef pair<int, int> ii;
typedef vector<ii> vii;
typedef vector<string> vs;
typedef double D;
typedef long double LD;
typedef long long ll;
typedef pair<ll, ll> pll;
typedef vector<ll> vll;

template<class T> T abs(T x) { return x > 0 ? x : -x;}

int m;
int n;

string s[10];
int col[10000];

int cur[10];

int getans(vi v) {
	if (sz(v) == 0)
		re 0;

	set<string> ss;
	ss.insert("");
	rep(i, sz(v)) {
		string o = s[v[i]];
		for (int j = 1; j <= sz(o); j++)
			ss.insert(o.substr(0, j));
	}
	re sz(ss);
}

void check() {
	vi v[10];
	rep(i, n) {
		v[cur[i]].pb(i);
	}
	int ans = 0;
	rep(i, m)
		ans += getans(v[i]);
	col[ans]++;
}

void go(int p) {
	if (p == n) {
		check();
		re;
	}

	rep(i, m) {
		cur[p] = i;
		go(p + 1);
	}
}

int main() {

#ifdef LOCAL_BOBER
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
#endif

	int tc;
	cin >> tc;
	rep(tt, tc) {
		cout << "Case #" << tt + 1 << ": ";

		cin >> n >> m;
		rep(i, n)
			cin >> s[i];

		fill(col, 0);
		go(0);

		rrep(i, 10000)
		if (col[i] > 0) {
			cout << i << ' ' << col[i] << endl;
			break;
		}
	}


	re 0;
}

