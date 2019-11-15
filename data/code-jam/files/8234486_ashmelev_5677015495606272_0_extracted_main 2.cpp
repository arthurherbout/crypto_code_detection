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
#define prev prev239
#define next next239
#define hash hash239
#define rank rank239
#define sqrt(x) sqrt(abs(x))
#define unq(x) (x.resize(unique(all(x)) - x.begin()))

using namespace std;

typedef vector<int> vi;
typedef vector<vi> vvi;
typedef pair<int, int> ii;
typedef vector<ii> vii;
typedef vector<string> vs;
typedef long long ll;
typedef double D;
typedef long double LD;
typedef pair<ll, ll> pll;

template<class T> T abs(T x) {
	return x > 0 ? x : -x;
}

int m;
int n;

vector<vs> v;

int matr[10][10];

vs update(vs v) {
	vector<vs> u;
	rep(i, sz(v)) {
		vs tmp;
		rep(j, sz(v)) {
			tmp.pb(v[(j + i) % sz(v)]);
		}
		u.pb(tmp);
	}
	re *min_element(all(u));
}

void checkv() {
	vs tmp;
	rep(i, m) {
		string s = "";
		rep(j, n)
			s += (char) (matr[j][i] + 48);
		tmp.pb(s);
	}
	v.pb(update(tmp));
}

int dx[4] = {1, 0, -1, 0};
int dy[4] = {0, 1, 0, -1};

int check(int x, int y, int f) {
	if (f == 1) {
		if (!check(x, y, 0))
			re 0;
		rep(o, 4) {
			int nx = (x + dx[o]);
			int ny = (y + dy[o] + m) % m;
			if (nx >= 0 && nx < n && matr[nx][ny] != 0)
				if (!check(nx, ny, 0))
					re 0;
		}
		re 1;
	}


	int val = matr[x][y];
	int c0 = 0, cval = 0;
	rep(o, 4) {
		int nx = (x + dx[o]);
		int ny = (y + dy[o] + m) % m;
		if (nx >= 0 && nx < n) {
			if (matr[nx][ny] == 0)
				c0++;
			if (matr[nx][ny] == val)
				cval++;
		}
	}
	//cout << x << ' ' << y << ' ' << val << ' ' << c0 << ' ' << cval << endl;
	if (cval > val)
		re 0;
	if (cval + c0 < val)
		re 0;
	re 1;
}

void go(int x, int y, int c) {

	//cout << x << ' ' << y << ' ' << c << endl;

	if (c == 0) {
		checkv();
		re;
	}

	for (int o = 1; o <= 3; o++) {
		matr[x][y] = o;
		if (check(x, y, 1)) {
			int nx = x + 1;
			int ny = y;
			if (nx == n) {
				nx = 0;
				ny++;
			}
			go(nx, ny, c - 1);
		}
		matr[x][y] = 0;
	}
}

int main() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	int tc;
	cin >> tc;
	rep(tt, tc) {
		cout << "Case #" << tt + 1 << ": ";

		cin >> n >> m;

		v.clear();
		fill(matr, 0);
		go(0, 0, n * m);

		sort(all(v));
		unq(v);
		cout << sz(v) << endl;
	}

	re 0;
}
