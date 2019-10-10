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

short matr[6000][6000];
short t[6000][6000];

int dx[6] = {1, 0, -1, -1, 0, 1};
int dy[6] = {1, 1, 0, -1, -1, 0};

int check(int x, int y) {
	re x >= 0 && y >= 0 && x <= 2 * n - 2 && y <= 2 * n - 2 && abs(x - y) < n;
}

vii v;

string gets(int x) {
	string ans;
	if (x & 1)
		ans += "bridge";
	if (x & 2) {
		if (sz(ans))
			ans += '-';
		ans += "fork";
	}
	if (x & 4) {
		if (sz(ans))
			ans += '-';
		ans += "ring";
	}
	re ans;
}

int w[6000][6000];
int ct;
int m1, m2;
int loh;

int go2(int x, int y) {
	w[x][y] = ct;
	//cout << x << ' ' << y << endl;
	rep(i, 6) {
		int nx = x + dx[i];
		int ny = y + dy[i];

		if (!check(nx, ny)) {
			loh = 1;
		}
		else
			if (matr[x][y] == matr[nx][ny] && w[nx][ny] != ct)
				go2(nx, ny);
	}
	re 0;
}

int getp(int x) {
	re __builtin_popcount(x);
}

void go1(int x, int y) {
	w[x][y] = ct;
	if (t[x][y] == 3)
		m1 = m1 * 2 + 1;
	if (t[x][y] == 4) {
		if (x == 0)
			m2 |= 1;
		if (y == x + n - 1)
			m2 |= 2;
		if (y == 2 * n - 2)
			m2 |= 4;
		if (x == 2 * n - 2)
			m2 |= 8;
		if (x == y + n - 1)
			m2 |= 16;
		if (y == 0)
			m2 |= 32;
	}
	rep(i, 6) {
		int nx = x + dx[i];
		int ny = y + dy[i];

		if (!check(nx, ny))
			continue;
		if (matr[x][y] == matr[nx][ny] && w[nx][ny] != ct)
			go1(nx, ny);
	}
}

int get(int p) {
	rep(i, sz(v)) {
		int x = v[i].fi;
		int y = v[i].se;
		if (i < p)
			matr[x][y] = 1;
		else
			matr[x][y] = 0;
	}
	ct++;

	int ans = 0;
	rep(i, p) {
		int x = v[i].fi;
		int y = v[i].se;
		if (w[x][y] != ct) {
			m1 = 0;
			m2 = 0;
			go1(x, y);
			if (getp(m1) >= 2)
				ans |= 1;
			if (getp(m2) >= 3)
				ans |= 2;
		}
	}

	rep(i, 2 * n) rep(j, 2 * n)
		if (matr[i][j] == 0 && w[i][j] != ct && check(i, j)) {
			loh = 0;
			go2(i, j);
			if (!loh) {
				//cout << i << ' ' << j << endl;
				ans |= 4;
				re ans;
			}
		}

	re ans;
}

int main() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	int tc;
	cin >> tc;
	rep(tt, tc) {
		cout << "Case #" << tt + 1 << ": ";

		cin >> n >> m;
		rep(i, 2 * n) rep(j, 2 * n) {
			if (check(i, j)) {
				int c = 0;
				rep(k, 6) {
					int nx = i + dx[k];
					int ny = j + dy[k];
					if (check(nx, ny))
						c++;
				}
				t[i][j] = c;
			}
			else
				t[i][j] = -1;
		}

		v.clear();
		rep(i, m) {
			int a, b;
			cin >> a >> b;
			v.pb(mp(a - 1, b - 1));
		}


		int l = 0, r = m;
		int sc = -1, sm = 0;
		for (int c = 0; c <= m; c++) {

		//while (l <= r) {
		//	int c = (l + r) / 2;
			int g = get(c);
			if (g > 0) {
				sm = g;
				sc = c;
				r = c - 1;
				break;
			}
			else
				l = c + 1;
		}

		rep(i, sz(v))
		matr[v[i].fi][v[i].se] = 0;

		if (sc == -1)
			cout << "none" << endl;
		else
			cout << gets(sm) << " in move " << sc << endl;
	}

	re 0;
}
