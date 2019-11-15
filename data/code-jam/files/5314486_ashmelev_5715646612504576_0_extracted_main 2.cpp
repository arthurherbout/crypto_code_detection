#include <iostream>
#include <cstdio>
#include <cstdlib>
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
#define next next239
#define prev prev239

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

int nint() {
	int x;
	scanf("%d", &x);
	re x;
}

string s[55];

int ok[2][55][55];

int use[55][55];
int bad[55][55];
int was[55][55];

int ans[55][55];

int dx[4] = {1, 0, -1, 0};
int dy[4] = {0, 1, 0, -1};

int good(int x, int y) {
	re x >= 0 && x < n && y >= 0 && y < m;
}

int check1(int x, int y, int p, bool fix) {
	while (1) {
		if (fix)
			was[x][y] = 1;
		int nx = x + dx[p];
		int ny = y + dy[p];
		if (!good(nx, ny))
			re 1;
		if (use[nx][ny])
			re 0;
		if (bad[nx][ny])
			re 1;
		x = nx;
		y = ny;
	}
	re 0;
}

int find1(int x, int y, int p) {
	while (1) {
		int nx = x + dx[p];
		int ny = y + dy[p];
		if (!good(nx, ny))
			re -1;
		if (was[nx][ny] < 0)
			re -was[nx][ny] - 1;
		if (bad[nx][ny])
			re -1;
		x = nx;
		y = ny;
	}
	re 0;
}

int check(int x, int y, int o) {
	if (o == 0)
		re check1(x, y, 0, 0) && check1(x, y, 2, 0);
	else
		re check1(x, y, 1, 0) && check1(x, y, 3, 0);
}

vii gv;
set<int> gs;
int beda;

void fix(int x, int y, int o) {
	//cout << "fix: " << x << ' ' << y << ' ' << o << endl;
	if (ans[x][y] != -1 && ans[x][y] != o)
		beda = 1;
	ans[x][y] = o;
	//was[x][y]
	if (o == 0) {
		check1(x, y, 0, 1);
		check1(x, y, 2, 1);
	}
	else {
		check1(x, y, 1, 1);
		check1(x, y, 3, 1);
	}
}

vi v[6000];
int gn;

void add(int x, int y) {
	//cout << "add: " << x << ' ' << y << endl;
	v[x].pb(y);
}

ii get(int x, int y) {
	int a = max(find1(x, y, 0), find1(x, y, 2));
	int b = max(find1(x, y, 1), find1(x, y, 3));

	//cout << "ab  " << a << ' ' << b << endl;

	if (a == -1 && b == -1) {
		re {-1, -1};
	}

	//cout << "get: " << x << ' ' << y << ' ' << a << ' ' << b << ' ' << endl;
	if (a == -1) {
		fix(gv[b].fi, gv[b].se, 1);
		//gs.insert(b);
	}
	if (b == -1) {
		fix(gv[a].fi, gv[a].se, 0);
		//gs.insert(a);
	}

	re {a, b};
}

int z[6000];
int zt;

int bro[6000];
int aa[6000];

void go(int x) {


	z[x] = zt;
	for (int y : v[x]) {
		if (z[y] != zt)
			go(y);
	}
}


void go2(int x) {

	//cout << "go2 " << x << ' ' << gn << endl;

	z[x] = zt;
	if (x >= gn)
		aa[x - gn] = 0;
	else
		aa[x] = 1;

	for (int y : v[x])
		if (z[y] != zt)
		go2(y);
}

int bu() {
	fill(aa, -1);

	rep(i, gn * 2) {
		int ni = i + gn;

		if (gs.count(i) || gs.count(ni))
			continue;

		zt++;
		for (int x : v[i]) {
			go(x);
		}

		if (i >= gn)
			ni = i - gn;
		bro[i] = (z[ni] == zt);

		//cout << i << ' ' << ni << ' ' << bro[i] << endl;
	}

	rep(i, gn) {

		//cout <<"iii" << i << ' ' << aa[i] << endl;
		//if (gs.count(i))
		//	continue;

		if (bro[i] && bro[i + gn])
			re 0;

		int x = gv[i].fi;
		int y = gv[i].se;

		if (aa[i] != -1) {
			if (ans[x][y] == aa[i])
				ans[x][y] = aa[i];
			continue;
		}

		//cout << x << ' ' << y << ' ' << i << ' ' << bro[i] << ' ' << bro[i + gn] << endl;

		/*
		if (!sz(v[i])) {
			if (ans[x][y] == -1)
				ans[x][y] = 0;
			continue;
		}*/

		//cout << "i = " << i << endl;

		zt++;
		if (bro[i]) {
			if (ans[x][y])
				beda = 1;
			ans[x][y] = 0;
			go2(i + gn);
		}
		else {
			if (ans[x][y] == 0)
				beda = 1;
			ans[x][y] = 1;
			go2(i);
		}
	}

	re 1;
}

void solve() {
	fill(ans, -1);
	beda = 0;
	gs.clear();
	rep(i, 6000)
		v[i].clear();

	cin >> n >> m;
	rep(i, n) {
		cin >> s[i];
	}

	rep(i, n) rep(j, m) {
		use[i][j] = (s[i][j] == '|' || s[i][j] == '-');
		bad[i][j] = s[i][j] == '#';
	}

	rep(i, n) rep(j, m)
		if (use[i][j]) {
			int a[2];
			rep(o, 2)
				a[o] = ok[o][i][j] = check(i, j, o);

			if (!a[0] && !a[1]) {
				cout << "IMPOSSIBLE" << endl;
				re;
			}
		}

	vii v;

	rep(i, n) rep(j, m)
		was[i][j] = bad[i][j] || use[i][j];

	rep(i, n) rep(j, m)
		if (use[i][j]) {
			int a[2];
			rep(o, 2)
				a[o] = ok[o][i][j];

			if (a[0] && !a[1])
				fix(i, j, 0);

			if (!a[0] && a[1])
				fix(i, j, 1);

			if (a[0] && a[1]) {
				//cout << i << ' ' << j << endl;
				v.pb({i, j});
			}
		}

	rep(i, sz(v)) {
		was[v[i].fi][v[i].se] = -(i + 1);
	}

	gv = v;
	gn = sz(v);

	/*
	cout << endl;
	rep(i, n) {
		rep(j, m) {
			cout << was[i][j] << ' ';
		}
		cout << endl;
	}
	cout << endl;
*/

	rep(i, n) rep(j, m)
	if (!was[i][j]) {
		//cout << i << ' ' << j << endl;

		ii o = get(i, j);
		//cout << "ok1" << endl;
		//cout << i << ' ' << j << ' ' << o.fi << ' ' << o.se << endl;

		if (o.fi == -1 && o.se == -1) {
			cout << "IMPOSSIBLE" << endl;
			re;
		}

		if (o.fi == -1) {
			add(o.se + sz(v), o.se);
		}

		if (o.se == -1) {
			add(o.fi, sz(v) + o.fi);
		}

		if (o.fi == -1 || o.se == -1)
			continue;


		//cout << o.fi << ' ' << o.se << endl;

		add(o.fi, o.se);
		add(o.se + sz(v), o.fi + sz(v));
	}

	//cout << "ok" << endl;

	if (beda || !bu()) {
		cout << "IMPOSSIBLE" << endl;
		re;
	}

	cout << "POSSIBLE" << endl;

	rep(i, n) {
		rep(j, m) {
			if (use[i][j])
			{
				if (!ans[i][j])
					cout << "|";
				else
					cout << "-";
			}
			else
				cout << s[i][j];
		}
		cout << endl;
	}
}

int main() {

#ifdef LOCAL_BOBER
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
#else
	//freopen("input.txt", "r", stdin);
	//freopen("output.txt", "w", stdout);
#endif

	int tc = 1;
	cin >> tc;
	rep(tt, tc) {
		cout << "Case #" << tt + 1 << ": ";
		solve();
	}


	re 0;
}
