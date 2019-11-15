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

template<class T> string toString(T x) {
	stringstream sout;
	sout << x;
	re sout.str();
}

int nint() {
	int x;
	scanf("%d", &x);
	re x;
}

int m;
int n;
int ans;

int was[100][100];
int a[100][100];
int ct;
vii gv;
int cn, cm;
int ok[100][100];
int zlo[100][100];
int dno[100][100];

vii gg;

ii get(int a) {
	if (a < m)
		re {0, 2 * a + 1};
	a -= m;
	if (a < n)
		re {2 * a + 1, 2 * m};
	a -= n;
	if (a < m)
		re {2 * n, 2 * (m - a) - 1};
	a -= m;
	re {2 * (n - a) - 1, 0};
}

int dx[8] = {1, 0, -1, 0, 1, 1, -1, -1};
int dy[8] = {0, 1, 0, -1, 1, -1, 1, -1};

void go(int x, int y) {
	//cout << x << ' ' << y << endl;

	if (zlo[x][y])
		gg.pb({x, y});
	was[x][y] = ct;

	rep(i, 8) {
		int nx = x + dx[i];
		int ny = y + dy[i];
		if (nx >= 0 && nx < cn && ny >= 0 && ny < cm) {
			if (i >= 4 && ok[x][ny] == 0 && ok[nx][y] == 0 && (dno[x][ny] & dno[nx][y]))
				continue;
			if (ok[nx][ny] && was[nx][ny] != ct) {
				go(nx, ny);
			}
		}
	}
}

int check(ii a, ii b) {
	ct++;
	gg.clear();
	go(a.fi, a.se);

	if (sz(gg) != 2 || gg.back() != b)
		re 0;
	re 1;
}

int check(int mask) {
	rep(i, n) rep(j, m) {
		int p = i * m + j;
		if (mask & (1 << p))
			a[i][j] = 1;
		else
			a[i][j] = 0;
	}
	cn = 2 * n + 1;
	cm = 2 * m + 1;

	rep(i, cn) rep(j, cm) {
		ok[i][j] = 1;
		dno[i][j] = 0;
	}


	int u = 0;
	rep(i, n) rep(j, m) {
		u++;
		if (a[i][j]) {
			ok[2 * i][2 * j + 2] = 0;
			ok[2 * i + 1][2 * j + 1] = 0;
			ok[2 * i + 2][2 * j] = 0;

			dno[2 * i][2 * j + 2] |= (1 << u);
			dno[2 * i + 1][2 * j + 1] |= (1 << u);
			dno[2 * i + 2][2 * j] |= (1 << u);
		}
		else {
			ok[2 * i][2 * j] = 0;
			ok[2 * i + 1][2 * j + 1] = 0;
			ok[2 * i + 2][2 * j + 2] = 0;

			dno[2 * i][2 * j] |= (1 << u);
			dno[2 * i + 1][2 * j + 1] |= (1 << u);
			dno[2 * i + 2][2 * j + 2] |= (1 << u);

		}
	}

	if (0)
	rep(i, cn) {
		rep(j, cm)
			cout << dno[i][j] << ' ';
		cout << endl;
	}

	rep(i, sz(gv)) {
		ii a = get(gv[i].fi);
		ii b = get(gv[i].se);
		//cout << gv[i].fi << ' ' << gv[i].se << ' ' << a.fi << ' ' << a.se << ' ' << b.fi << ' ' << b.se << endl;
		if (!check(a, b))
			re 0;
	}
	re 1;
}

int main() {
#ifdef LOCAL_BOBER
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
#else
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
#endif


	int tc;
	cin >> tc;
	rep(tt, tc) {
		cout << "Case #" << tt + 1 << ":" << endl;;
		cin >> n >> m;
		vii v;
		rep(i, n + m) {
			int a, b;
			cin >> a >> b;
			a--;
			b--;
			v.pb({a, b});
		}

		int g = 0;
		gv = v;

		fill(zlo, 0);
		rep(i, 2 * (n + m)) {
			ii o = get(i);
			zlo[o.fi][o.se] = 1;
		}

		rep(i, (1 << (n * m)))
		//rep(i, 1)
		if (check(i)) {
			g = 1;
			rep(i, n) {
				rep(j, m)
					if (a[i][j])
						cout << '/';
					else
						cout << '\\';
				cout << endl;
			}
			break;
		}
		if (!g)
			cout << "IMPOSSIBLE" << endl;
	}


	re 0;
}










