#include <iostream>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <string>
#include <vector>
#include <queue>
#include <set>
#include <bitset>
#include <deque>
#include <map>
#include <stack>
#include <sstream>
#include <cstring>
#include <cstdio>
#include <cstdlib>

#define mp make_pair
#define pb push_back
#define all(x) (x).begin(), (x).end()
#define sz(x) ((int)((x).size()))
#define sqr(x) ((x)*(x))
#define For(i,a,b) for(int i = (a); i < (b); i++)
#define rep(i,n) For(i,0,n)
#define rrep(i,n) for (int i = ((n) - 1); i >= 0; i--)
#define re return
#define fi first
#define se second
#define y0 y47847892
#define y1 y47824262
#define fill(x, val) memset(x, val, sizeof(x))

using namespace std;

typedef vector<int> vi;
typedef vector<string> vs;
typedef pair<int, int> ii;
typedef vector<ii> vii;
typedef vector<vi> vvi;
typedef long long ll;

template<class T> T abs(T x) { return x > 0 ? x : -x;}

int n;
int m;
int b, col;
int c[100];

int mod = 1000000007;

int razl[2500][71][71];

int getrazl(int sum, int k, int m) {
	if (sum >= 2500)
		re 0;
	if (!sum) {
		if (!k)
			re 1;
		else
			re 0;
	}
	if (!m || !k)
		re 0;

	int &ans = razl[sum][k][m];
	if (ans != -1)
		re ans;
	ans = getrazl(sum, k, m - 1);
	if (sum >= m)
		ans += getrazl(sum - m, k - 1, m - 1);

	//cout << sum << ' ' << k << ' ' << m << ' ' << ans << endl;

	ans %= mod;
	re ans;
}

int cnk[100][100];

int table[100][80][80];

vvi ttt[101][101];
int ww[101][101];

vvi raz(int x, int m) {
	vvi ans;

	if (x == 0 || m == 0)
		re ans;

	if (ww[x][m])
		re ttt[x][m];

	ww[x][m] = 1;
	for (int u = m; u >= 1; u--) {
		if (u > x)
			continue;

		if (u == x) {
			vi tmp;
			tmp.pb(x);
			ans.pb(tmp);
		}
		vvi z = raz(x - u, u - 1);
		rep(i, sz(z)) {
			z[i].pb(u);
			ans.pb(z[i]);
		}
	}

	ttt[x][m] = ans;
	re ans;
}

int getans(int p, int d, int t, int u) {
	//rep(i, u)
	//		cout << "  ";
	//cout << p << ' ' << d << ' ' << t << endl;
	if (p == col) {
		re (d == 0 && t == 0);
	}
	if (t == 0) {
		if (p == col - 1 && d == c[p])
			re 1;
		re 0;
	}

	int &ans = table[p][d][t];
	if (ans != -1)
		re ans;
	ans = 0;
	//cout << p << ' ' << c[p] << endl;
	for (int k = 0; k <= t; k++) {
		for (int d1 = 0; d1 <= t; d1++) {
			int nval = d1 * b + c[p] - d;
			if (nval < 0)
				continue;

			// 0
			if (k) {
				ll col1 = getrazl(nval, t - 1, b - 1);
				//cout << nval << ' ' << t - 1 << ' ' << col1 << endl;
				ll tmp = cnk[t][k - 1] * col1;
				tmp %= mod;
				tmp *= getans(p + 1, d1, k, u + 1);
				ans = (ans + (tmp % mod)) % mod;

				//cout << p << ' ' <<  k << ' ' << t << ' ' << d << ' ' << d1 << ' ' << tmp << endl;
			}

			ll col1 = getrazl(nval, t, b - 1);
			ll tmp = cnk[t][k] * col1;
			//cout << nval << ' ' << t << ' ' << col1 << ' ' << tmp << endl;
			tmp %= mod;
			tmp *= getans(p + 1, d1, k, u + 1);
			ans = (ans + (tmp % mod)) % mod;
		}
	}

	re ans % mod;
}

int used[100][100];

int check(vi v, int b) {
	fill(used, 0);
	rep(i, sz(v)) {
		int x = v[i];
		int p = 0;
		while (x) {
			if (used[x % b][p])
				re 0;
			used[x % b][p] = 1;
			p++;
			x /= b;
		}
	}

	re 1;
}

int main() {

#ifndef ONLINE_JUDGE
	freopen("test.in", "r", stdin);
	freopen("test.out", "w", stdout);
#endif


	int tc;
	cin >> tc;

	cnk[0][0] = 1;
	for (int i = 1; i < 100; i++) {
		cnk[i][0] = 1;
		for (int j = 1; j < 100; j++)
			cnk[i][j] = (cnk[i - 1][j] + cnk[i - 1][j - 1]) % mod;
	}

	fill(razl, -1);

	//b = 4;
	//cout << "r = " << getrazl(1, 1, 3);
	//cout << getrazl(5, 2, 4) << endl;
	//re 0;
/*
	fill(table, -1);
	col = 2;
	c[1] = 2;
	cout << getans(0, 0, 2, 0) << endl;
	re 0;*/

	rep(tt, tc) {
		cout << "Case #" << tt + 1 << ": ";
		ll x;
		ll sx;

		cin >> x >> b;
		sx = x;
		ll ans = 0;

		vvi u= raz(x, x);
		ans = 0;
		rep(i, sz(u)) {
			if (check(u[i], b))
				ans++;
			//rep(j, sz(u[i]))
				//cout << u[i][j] << ' '
		}
		cout << ans << endl;
		continue;


		col = 0;
		while (x) {
			c[col++] = x % b;
			x /= b;
		}

		if (0) {
		cout << col << endl;
		rep(i, col)
			cout << c[i] << ' ';
		cout << endl;
		}

		if (0)
		rep(i, 10) {
			rep(j, 10)
				cout << cnk[i][j] << ' ';
			cout << endl;
		}

		fill(table, -1);
		for (int t = 1; t <= b; t++) {
			cout << t << ' ' << getans(0, 0, t, 0) << endl;
			ans += getans(0, 0, t, 0);
		}

		cout << sx << ' ' << b << ' ' << ans << endl;
		//cout << ans % mod << endl;
	}

	return 0;
}
